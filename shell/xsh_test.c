/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>
#include <w25qxxx.h>
#include "emulator.h"
#include "mem.h"



shellcmd xsh_test(int nargs, char *args[])
{




   /* uint32 val = 0xbebecafe;
    cache_write( 0, &val, 4 ); 
	cache_read( 0, &val, 4 );
	if(val!=0xbebecafe){
		printf("error addr 0\n");
		return -1;
	}

    cache_write( 1000, &val, 4 ); 
	cache_read( 1000, &val, 4 );
    if(val!=0xbebecafe){
		printf("error addr 1000\n");
		return -1;
	}

    cache_write( 500000, &val, 4 ); 
	cache_read(  500000, &val, 4 );
    if(val!=0xbebecafe){
		printf("error addr 500000\n");
		return -1;
	}*/

   //cachebegin(4000000);
	//load_sd_file( 0, "riscv/kernel.bin" );
   
    /*uint8 d = cacheread(0x219d0);
    printf("%02x\n",d);

    for (int i = 0; i < 512; ++i)
    {
    	 
    	 for (int j = 0; j < 32; ++j)
    	 {
    	 	   
    	 }
    }*/
    /*for (int i = 0; i < 64; ++i)
    {

    	uint16 r;
    	cache_read( i, &r, 2 );
    	printf("%x\n", r);
    }*/


    printf( "\nStarting RISC-V VM\n\n\r" );
	int c = riscv_emu();
	while ( c == EMU_REBOOT ) c = riscv_emu();
	/*cachebegin(4000000);

	for (int i = 0; i < 500000; ++i)
    {
        cachewrite(i,0x33);
    }
    cachewrite(500000,0xbe);
    cachewrite(500000-1,0xbe);
    cachewrite(500000-2,0xbe);
    cachewrite(500000-3,0xca);
    cachewrite(500000-4,0xfe);
 
    uint8 c = cacheread(500000);
    printf("%02x\n",c);


    c = cacheread(0);
    printf("%02x\n",c);

    c = cacheread(500000-1);
    printf("%02x\n",c);

    c = cacheread(500000-2);
    printf("%02x\n",c);

    c = cacheread(500000-3);
    printf("%02x\n",c);

    c = cacheread(500000-4);
    printf("%02x\n",c);*/

	return 0;
}
