/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>
#include <w25qxxx.h>


shellcmd xsh_format(int nargs, char *args[])
{
	flash_info_t *flash_info;
    flash_info = w25qxxx_drv.getcardinfo();

    printf("formating...");
    if (fl_format(flash_info->card_size, "")){
         printf("format ok\n");
         return 0;
    }else{
        printf("error formating...");
    }
	return 0;
}
