
#include <stdint.h>



#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128  0XEF17
#define SPI_FLASH_SECTOR_COUNT   (31250/*-3906*/)//16m/512=31250// aqui se le restaran 2m que es igual a 2m/512=3906
#define FLASH_SECTOR_SIZE  512 
#define FLASH_SECTOR_SIZE4K  4096 
//#define	SPI_FLASH_CS PCout(4)  //选中FLASH	
				 
////////////////////////////////////////////////////////////////////////////
 
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void SPI_Flash_Init(void);
uint16_t  SPI_Flash_ReadID(void);  	    //读取FLASH ID
uint8_t	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(uint8_t sr);  	//写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void);	//写保护
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);			  //唤醒

extern uint16_t SPI_FLASH_TYPE;//定义我们使用的flash芯片型号

typedef struct flash_info
{
	uint32_t   card_size;       // card size in sectors
	uint32_t   sect_size;       // sector size in bytes
} flash_info_t;

flash_info_t* flash_spi_getcardinfo(void);

typedef struct w25qxxx_drv
{
	void (*init) (void);
	unsigned char (*read) (uint8_t *rxbuf, uint32_t sector, uint32_t count);
	unsigned char (*write) (const uint8_t *txbuf, uint32_t sector, uint32_t count);
	flash_info_t* (*getcardinfo) (void);
} w25qxxx_drv_t;


typedef struct w25qxxx_drv4K
{
	unsigned char (*read) (uint8_t *rxbuf, uint32_t sector, uint32_t count);
	unsigned char (*write) (const uint8_t *txbuf, uint32_t sector, uint32_t count);
} w25qxxx_drv_t4K;
extern const w25qxxx_drv_t w25qxxx_drv;

 


#define BLOCK 0xfff
extern void cachebegin(uint32_t size);
extern void cachewrite(uint32_t address,uint8 value);
extern uint8_t cacheread(uint32_t address);
extern uint32_t cacheget_size();

extern uint8_t lowmemRAM[];
extern uint8_t cacheRAM1[];
extern uint8_t cacheRAM2[];
//extern uint8_t cacheRAM3[];
//extern uint8_t cacheRAM4[];

extern uint32_t block1;
extern uint32_t block2;
//extern uint32_t block3;
//extern uint32_t block4;

extern uint8_t dirty1;
extern uint8_t dirty2;
//extern uint8_t dirty3;
//extern uint8_t dirty4;

extern uint8_t NextCacheUse;
extern uint32_t max;
