
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <gpio.h>
#include <syscall.h>
#include <elf.h>
#include <limits.h>
#define FILE            lfs_file_t
#define DIR             lfs_dir_t



syscall_t syscallp;


uint32_t ram_amt = 50000;
int fail_on_all_faults = 0;

static int64_t SimpleReadNumberInt( const char * number, int64_t defaultNumber );
static uint64_t GetTimeMicroseconds();
static void ResetKeyboardInput();
static void CaptureKeyboardInput();
static uint32_t HandleException( uint32_t ir, uint32_t retval );
static uint32_t HandleControlStore( uint32_t addy, uint32_t val );
static uint32_t HandleControlLoad( uint32_t addy );
static void HandleOtherCSRWrite( uint8_t * image, uint16_t csrno, uint32_t value );
static int32_t HandleOtherCSRRead( uint8_t * image, uint16_t csrno );
static void MiniSleep();
 

// This is the functionality we want to override in the emulator.
//  think of this as the way the emulator's processor is connected to the outside world.
#define MINIRV32WARN( x... ) printf( x );
#define MINIRV32_DECORATE  static
#define MINI_RV32_RAM_SIZE ram_amt
#define MINIRV32_IMPLEMENTATION
#define MINIRV32_POSTEXEC( pc, ir, retval ) { if( retval > 0 ) { if( fail_on_all_faults ) { printf( "FAULT\n" ); return 3; } else retval = HandleException( ir, retval ); } }
#define MINIRV32_HANDLE_MEM_STORE_CONTROL( addy, val ) if( HandleControlStore( addy, val ) ) return val;
#define MINIRV32_HANDLE_MEM_LOAD_CONTROL( addy, rval ) rval = HandleControlLoad( addy );
#define MINIRV32_OTHERCSR_WRITE( csrno, value ) HandleOtherCSRWrite( image, csrno, value );
#define MINIRV32_OTHERCSR_READ( csrno, value ) value = HandleOtherCSRRead( image, csrno );

#include "rv.h"


struct MiniRV32IMAState * core;
const char * kernel_command_line = 0;

static void DumpState( struct MiniRV32IMAState * core, uint8_t * ram_image );

int main(int argc, char *argv[])
{
	syscall_init(&syscallp);

	if(argc <2)return -1;

	uint8_t * ram_image;
    

    {
        char *tmp=syscallp.full_path(argv[1]);
	    if (tmp==NULL)return -1;
		if(!syscallp.exist(tmp)){
	       printf("%s not found!\n",tmp );
	       syscallp.updatepath();
	       return -1;
	    }
	    ram_image = syscallp.malloc( ram_amt );
	    if( ram_image==NULL ){
		    printf("Error: could not allocate system image.\n" );
		    return -4;
	    }
		FILE *fs=syscallp.open(tmp,LFS_O_RDWR);
		int len = syscallp.size(fs);
        syscallp.read(ram_image,len,fs);
        syscallp.close(fs);
        printf("%s\n",tmp );
    }
    restart:
    printf("iniciando ...\n");
    core = (struct MiniRV32IMAState *)(ram_image + ram_amt - sizeof( struct MiniRV32IMAState ));
	core->pc = MINIRV32_RAM_IMAGE_OFFSET;
	core->regs[10] = 0x00; //hart ID
	core->regs[11] = 0; //dtb_pa (Must be valid pointer) (Should be pointer to dtb)
	core->extraflags |= 3; // Machine-mode.

	
	// Image is loaded.
	uint64_t rt;
	uint64_t lastTime = 0;// (GetTimeMicroseconds()/time_divisor;
	int instrs_per_flip = 1024;//single_step?1:1024;

    while(1){

	  //scanf("%i",&c);
	  //for (int i = 0; i < c; ++i)
	  {
	  	int ret = MiniRV32IMAStep( core, ram_image, 0, 1, instrs_per_flip ); // Execute upto 1024 cycles before breaking out.
        switch( ret )
		{
			case 0: break;
			case 1: 
				 //wfi
			break;
			//case 3: instct = 0; break;
			case 0x7777: goto restart;	//syscon code for restart
			case 0x5555: 
				{
			     printf( "POWEROFF@0x%08x%08x\n", core->cycleh, core->cyclel ); 
			     printf("exit\n");
                 syscallp.free(ram_image);
                 syscallp.updatepath();
			     return 0;
			    } //syscon code for power-off
			break;
			default: printf( "Unknown failure %d\n",ret ); break;
		}
	  }
	  	  
	}
	printf("exit\n");
    syscallp.free(ram_image);
    syscallp.updatepath();
	return 0;
}




//////////////////////////////////////////////////////////////////////////
// Rest of functions functionality
//////////////////////////////////////////////////////////////////////////

static uint32_t HandleException( uint32_t ir, uint32_t code )
{
	// Weird opcode emitted by duktape on exit.
	if( code == 3 )
	{
		// Could handle other opcodes here.
	}
	return code;
}

static uint32_t HandleControlStore( uint32_t addy, uint32_t val )
{
	if( addy == 0x10000000 ) //UART 8250 / 16550 Data Buffer
	{
		printf( "%c", val );
	}
	return 0;
}


static uint32_t HandleControlLoad( uint32_t addy )
{
	// Emulating a 8250 / 16550 UART
	if( addy == 0x10000005 )
		return 0;//kbhit();
	else if( addy == 0x10000000 /*&& kbhit() */)
		return 0;//readbyte();
	return 0;
}

static void HandleOtherCSRWrite( uint8_t * image, uint16_t csrno, uint32_t value )
{

	if( csrno == 0x402 ){
		printf( "%c", value );  
	}else if( csrno == 0x136 )
	{
		printf( "%d", value );  
	}else if( csrno == 0x137 )
	{
		printf( "%08x", value );  
	}
}

static int32_t HandleOtherCSRRead( uint8_t * image, uint16_t csrno )
{
	if( csrno == 0x140 )
	{
		//if( !kbhit() ) return -1;
		//return readbyte();
	}
	return 0;
}

