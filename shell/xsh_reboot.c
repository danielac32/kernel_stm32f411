/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stm32.h>


#define VARADDR 0x0800C000 // address that will hold the variable
#define VAR     0x12345670 // variable value


/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
 
shellcmd xsh_reboot(int nargs, char *args[])
{
    //char *buff=malloc(200);
    
    //sprintf(buff,"hola como estas %d \n",888);
    
    //__syscall(XINU_TEST2,"hola como estas %d \n",888);
    //__syscall(XINU_TEST2,buff);
   	
   	//printf2("hola como estas %d \n",888);
    //printf("%s\n",buff );
    //free(buff);


    /*unlock_flash();
    erase_flash_sector(10);
    write_flash(0x08010000, 0xbebecafe);

    ///write_flash(VARADDR, 0xbebecafe);
        // do read write
    lock_flash();

    unlock_flash();

    uint32 r = read_flash(0x08010000);
    printf2("%08x\n", r);*/

   // uint32 r = syscall3(XINU_TEST2,0,0,0);
    //printf("%d\n", r);

    NVIC_SystemReset();
	return 0;
}
