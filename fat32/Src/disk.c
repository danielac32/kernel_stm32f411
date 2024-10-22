#include <disk.h>
#include <w25qxxx.h>
#include "sd-spi.h"
#include <stdio.h>
#include <xinu.h>

 
extern const sd_card_drv_t sd_card_drv;
extern const w25qxxx_drv_t w25qxxx_drv;




int sd_init(void)
{
   // disk_initialize(0);
    /*sd_card_drv.init();
    if (sd_card_drv.reset() != sd_err_ok)
    {
        kprintf("error init memory\n");
        while(1);
    }
    sd_info_t *sd_info;
    sd_info = sd_card_drv.getcardinfo();
    return sd_info->card_size;*/
    w25qxxx_drv.init();

    flash_info_t *flash_info;
    flash_info = w25qxxx_drv.getcardinfo();
    return flash_info->card_size;//return control(FLASHSPI,0,0,0);//flash_info->card_size;
}
//-----------------------------------------------------------------
// sd_readsector: Read a number of blocks from SD card
//-----------------------------------------------------------------
int sd_readsector(unsigned int start_block, unsigned char *buffer, unsigned int sector_count)
{
    //disk_read (0, buffer, start_block, sector_count);
   //sd_card_drv.read(buffer, start_block, sector_count);
    w25qxxx_drv.read(buffer, start_block, sector_count);
    //readSector(FLASHSPI,buffer,start_block,sector_count);
    return 1;
}
//-----------------------------------------------------------------
// sd_writesector: Write a number of blocks to SD card
//-----------------------------------------------------------------
int sd_writesector(unsigned int start_block, unsigned char *buffer, unsigned int sector_count)
{
    //disk_write (0, buffer, start_block, sector_count);
   //sd_card_drv.write(buffer, start_block, sector_count);
    
    w25qxxx_drv.write(buffer, start_block, sector_count);
    //writeSector(FLASHSPI,buffer,start_block,sector_count);
    return 1;
}




