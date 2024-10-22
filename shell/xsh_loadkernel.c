



/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>
#include <w25qxxx.h>
#include "emulator.h"




shellcmd xsh_loadkernel(int nargs, char *args[])
{
	load_sd_file( 0, "riscv/kernel.bin" );
  
	return 0;
}
