#include <xinu.h>
#include <w25qxxx.h>
#include <spi.h>


devcall	initFlash( struct dentry *devptr){
	/*static flash_info_t flashinfo;
	hal_w25q_spi_init();
	SPI_Flash_Init();
	flash_info_t *flash_info;
	flash_info = w25qxxx_drv.getcardinfo();
	return flash_info->card_size;*/
	//SPI_Flash_Init();
	w25qxxx_drv.init();
	return OK;
}