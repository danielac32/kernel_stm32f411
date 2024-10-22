#include <xinu.h>
#include <w25qxxx.h>
#include <gpio.h>
#include <fat_filelib.h>
#include <usb_cdc_conf.h>
#include "emulator.h"
#include "mem.h"

 
Memory mem;
int time_divisor = 256;
uint8_t fast_mode = 1;

static uint32_t HandleException( uint32_t ir, uint32_t retval );
static uint32_t HandleControlStore( uint32_t addy, uint32_t val );
static uint32_t HandleControlLoad( uint32_t addy );
static void HandleOtherCSRWrite( uint8_t *image, uint16_t csrno, uint32_t value );
static uint32_t HandleOtherCSRRead( uint8_t *image, uint16_t csrno );

#define INSTRS_PER_FLIP 1024
#define MICROSECOND_TICKS cycleCount() 

#define MINIRV32WARN( x... ) printf( x );
#define MINIRV32_DECORATE static
#define MINI_RV32_RAM_SIZE 126000
#define MINIRV32_IMPLEMENTATION
#define MINIRV32_POSTEXEC( pc, ir, retval )             \
	{                                                   \
		if ( retval > 0 )                               \
				retval = HandleException( ir, retval ); \
	}

#define MINIRV32_HANDLE_MEM_STORE_CONTROL( addy, val ) if ( HandleControlStore( addy, val ) ) return val;
#define MINIRV32_HANDLE_MEM_LOAD_CONTROL( addy, rval ) rval = HandleControlLoad( addy );
#define MINIRV32_OTHERCSR_WRITE( csrno, value ) HandleOtherCSRWrite( image, csrno, value );
#define MINIRV32_OTHERCSR_READ( csrno, rval )  { rval = HandleOtherCSRRead( image, csrno ); }

#define MINIRV32_CUSTOM_MEMORY_BUS

 
static void MINIRV32_STORE4(uint32_t ofs, uint32_t val)
{
 
	cache_write( ofs, &val, 4 );
	 
}

static void MINIRV32_STORE2(uint32_t ofs, uint16_t val)
{
	 
	cache_write( ofs, &val, 2 );
	 
}

static void MINIRV32_STORE1(uint32_t ofs, uint8_t val)
{
	 
	cache_write( ofs, &val, 1 );
 
}


static  uint32_t MINIRV32_LOAD4( uint32_t ofs )
{



	uint32_t val;
	 
	cache_read( ofs, &val, 4 );
	 
	return val;
}

static  uint16_t MINIRV32_LOAD2( uint32_t ofs )
{
	uint16_t val;
	 
	cache_read( ofs, &val, 2 );
	 
	return val;
}

static  uint8_t MINIRV32_LOAD1( uint32_t ofs )
{
	uint8_t val;
	 
	cache_read( ofs, &val, 1 );
	 
	return val;
}


#include "mini-rv32ima.h"

struct MiniRV32IMAState *core;


int riscv_emu()
{
	mem = create_memory("riscv/kernel.bin");
	int coresize32 = 0;
	core = (struct MiniRV32IMAState*)malloc(sizeof(struct MiniRV32IMAState));
	coresize32 = sizeof(struct MiniRV32IMAState) / 4;      // Number of UInt32 in core struct

	  // Clear the struct
	for (int i = 0; i < coresize32; i++) {
	    *(uint32_t*)((uint8_t*)core + 4*i) = 0;
	}

   // cache_reset();
	core->regs[10] = 0x00; // hart ID
	core->regs[11] = 0;
	core->extraflags |= 3; // Machine-mode.

	core->pc = MINIRV32_RAM_IMAGE_OFFSET;
	long long instct = -1;
    int instrs_per_flip = 1024;
	#if 1
        uint64_t lastTime = cycleCount();
		while(1){
			
			int ret;
			uint64_t *this_ccount = ((uint64_t*)&core->cyclel);
			uint32_t elapsedUs = cycleCount() / lastTime;

			lastTime += elapsedUs;
			//uint32 q=disable();
			ret = MiniRV32IMAStep( core, NULL, 0, elapsedUs, instrs_per_flip ); // Execute upto 1024 cycles before breaking out.
			//restore(q);
			switch ( ret )
			{
				case 0: break;
				case 1:
					 sleepms(1);
					break;
				case 3: instct = 0; break;
				case 0x7777:
					printf( "\n\rREBOOT@0x%08x%08x\n\r", (unsigned int)core->cycleh,  (unsigned int)core->cyclel );
					 
					//cache_reset();
					free(core);
					return EMU_REBOOT; // syscon code for reboot
				case 0x5555:
					printf( "\n\rPOWEROFF@0x%08x%08x\n\r",  (unsigned int)core->cycleh,  (unsigned int)core->cyclel );
					free(core);
					return EMU_POWEROFF; // syscon code for power-off
				default:
					printf( "\n\rUnknown failure\n" );
					free(core);
					return EMU_UNKNOWN;
					break;
			}
	
		}

	#else
    uint64_t rt;
	uint64_t lastTime = MICROSECOND_TICKS / time_divisor;
	
	for ( rt = 0; rt < instct + 1 || instct < 0; rt += INSTRS_PER_FLIP )
	{
		if(fast_mode)
			time_divisor = 8;

		uint64_t *this_ccount = ( (uint64_t *)&core.cyclel );

		uint32_t elapsedUs = MICROSECOND_TICKS / (time_divisor - lastTime);
		lastTime += elapsedUs;

		int ret = MiniRV32IMAStep( &core, NULL, 0, elapsedUs, INSTRS_PER_FLIP ); // Execute upto 1024 cycles before breaking out.
		switch ( ret )
		{
			case 0: break;
			case 1:
				//Delay_Ms( 1 );
				*this_ccount += INSTRS_PER_FLIP;
				break;
			case 3: instct = 0; break;
			case 0x7777:
				printf( "\n\rREBOOT@0x%08x%08x\n\r", (unsigned int)core.cycleh,  (unsigned int)core.cyclel );
				time_divisor = 256;
				fast_mode = 0;
				//cache_reset();
				return EMU_REBOOT; // syscon code for reboot
			case 0x5555:
				printf( "\n\rPOWEROFF@0x%08x%08x\n\r",  (unsigned int)core.cycleh,  (unsigned int)core.cyclel );
				return EMU_POWEROFF; // syscon code for power-off
			default:
				printf( "\n\rUnknown failure\n" );
				return EMU_UNKNOWN;
				break;
		}
	}

	#endif
	free(core);
	return EMU_UNKNOWN;
}

//////////////////////////////////////////////////////////////////////////
// Functions for the emulator
//////////////////////////////////////////////////////////////////////////

// Exceptions handling

static uint32_t HandleException( uint32_t ir, uint32_t code )
{
	// Weird opcode emitted by duktape on exit.
	if ( code == 3 )
	{
		// Could handle other opcodes here.
	}
	return code;
}

// CSR handling (Linux HVC console)


int load_sd_file( uint32_t addr, const char filename[] )
{
    FILE* fd;
    //uint32_t q=disable();
    //mem = create_memory("riscv/kernel.bin");
    char *tmp=full_path(filename);
    if (tmp==NULL){
        //restore(q);
        return -1;
    }
    if (!(fd = fopen(tmp,"r"))){
        printf("%s not found\n", tmp);
       // restore(q);
        return -1;
    }
    uint8 *buff=malloc(1024+1);
    unsigned int part=0;
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    printf("size file %d\n",fileLength);
    
    printf( "Loading image into RAM\n\r" );
    uint32 br=0;
   

    #if 1
    while(part<fileLength){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         br=fread(buff, 1024,1,fd);
         memory_write( addr+part,buff, br );
         memset(buff,0,br);
         printf("%d->%d\n",part, fileLength);
         part += br;
         hw_toggle_pin(GPIOx(GPIO_C),13);
    }
    free(buff);
    #else
        while(!feof(fd)){
              char c = fgetc(fd);
              cachewrite(addr,c);
              uint8 r= cacheread(addr);
              if(r!=c){
              	printf("error writing..\n");
              	break;
              }
              addr++;
              hw_toggle_pin(GPIOx(GPIO_C),13);
        }
        printf("%d->%d\n", addr,fileLength);
        
    /*while(part<fileLength){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         br=fread(buff, 64,1,fd);
         //SPI_Flash_Write( buff,addr+part, br );
         for (int i = 0; i < br; ++i)
         {
         	cachewrite(addr+i,buff[i]);
         }
         memset(buff,0,br);
         printf("%d->%d\n",part, fileLength);
         addr+=br;
         part += br;
         hw_toggle_pin(GPIOx(GPIO_C),13);
    }*/

    #endif
    fclose(fd);
    update_path();
    printf("end load\n");
   // restore(q);
    return 0;
}


static inline void HandleOtherCSRWrite( uint8_t *image, uint16_t csrno, uint32_t value )
{
	if( csrno == 0x402 ){
		printf( "%c", value );
	}else if( csrno == 0x136 )
	{
		printf( "%d", value ); 
	}else if( csrno == 0x137 )
	{
		printf( "%08x", value ); 
	}else if (csrno == 0x401){//upload
		uint32_t start = value - MINIRV32_RAM_IMAGE_OFFSET;
		printf("%08x\n", start);
		//load_sd_file( start, "riscv/fs2.img" ); 
	}

}

static inline uint32_t HandleOtherCSRRead( uint8_t *image, uint16_t csrno )
{
	 

	return 0;
}

// MMIO handling (8250 UART)

static uint32_t HandleControlStore( uint32_t addy, uint32_t val )
{
	if ( addy == 0x10000000 ) // UART 8250 / 16550 Data Buffer
		fputc(val,CONSOLE);//putchar( val );

	return 0;
}

static uint32_t HandleControlLoad( uint32_t addy )
{
	char c;
	int len;

	if( addy == 0x10000005 ){
		len = read(CONSOLE, &c,1);
		if (len == EOF) {
			return 0;
		}else return 1;
		//return usb_available();
	}
	else if( addy == 0x10000000 ){
		read(CONSOLE,&c,1);
		return c;
	}

	return 0;
}