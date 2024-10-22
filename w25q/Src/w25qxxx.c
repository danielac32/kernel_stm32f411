
#include <spi.h>
#include <interrupt.h>
#include <w25qxxx.h>
#include <gpio.h>

static flash_info_t flashinfo;
uint16_t SPI_FLASH_TYPE=W25Q128;//默认就是25Q16


void SPI_Flash_Init(void)
{   
   __disable_irq();       
    hal_w25q_spi_init();
    hal_w25q_spi_release();
    SPI_FLASH_TYPE=SPI_Flash_ReadID();//读取FLASH ID.  
    flashinfo.sect_size = 512;
    flashinfo.card_size = SPI_FLASH_SECTOR_COUNT;//16000000/512
    //hw_toggle_pin(GPIOx(GPIO_C),13);
    __enable_irq();
}  

uint8_t SPI_Flash_ReadSR(void)   
{  
    uint8_t byte=0;   
    __disable_irq();       
    hal_w25q_spi_select();                            //使能器件   
    hal_w25q_spi_txrx(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
    byte=hal_w25q_spi_txrx(0Xff);             //读取一个字节  
    hal_w25q_spi_release();     
     __enable_irq();                       //取消片选     
    return byte;   
} 
 
void SPI_FLASH_Write_SR(uint8_t sr)   
{   
    hal_w25q_spi_select();                            //使能器件   
    hal_w25q_spi_txrx(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
    hal_w25q_spi_txrx(sr);               //写入一个字节  
    hal_w25q_spi_release();                            //取消片选             
}   
   
void SPI_FLASH_Write_Enable(void)   
{
    hal_w25q_spi_select();                            //使能器件   
    hal_w25q_spi_txrx(W25X_WriteEnable);      //发送写使能  
    hal_w25q_spi_release();                            //取消片选             
} 
  
void SPI_FLASH_Write_Disable(void)   
{  
    hal_w25q_spi_select();                            //使能器件   
    hal_w25q_spi_txrx(W25X_WriteDisable);     //发送写禁止指令    
    hal_w25q_spi_release();                            //取消片选             
}               
 
uint16_t SPI_Flash_ReadID(void)
{
    uint16_t Temp = 0;    
    hal_w25q_spi_select();                  
    hal_w25q_spi_txrx(0x90);//发送读取ID命令      
    hal_w25q_spi_txrx(0x00);        
    hal_w25q_spi_txrx(0x00);        
    hal_w25q_spi_txrx(0x00);                   
    Temp|=hal_w25q_spi_txrx(0xFF)<<8;  
    Temp|=hal_w25q_spi_txrx(0xFF);   
    hal_w25q_spi_release();                 
    return Temp;
}               
 
void SPI_Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
  uint16_t i;   
  __disable_irq();                                                   
  hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_ReadData);         //发送读取命令   
  hal_w25q_spi_txrx((uint8_t)((ReadAddr)>>16) & 0xff); //发送24bit地址    
  hal_w25q_spi_txrx((uint8_t)((ReadAddr)>>8) & 0xff);   
  hal_w25q_spi_txrx((uint8_t)ReadAddr & 0xff);   
  for(i=0;i<NumByteToRead;i++)
  { 
      pBuffer[i]=hal_w25q_spi_txrx(0XFF);   //循环读数  
  }
    hal_w25q_spi_release();   
  __enable_irq();                         //取消片选             
}  

void SPI_Flash_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t i;  
  __disable_irq();  
  SPI_FLASH_Write_Enable();                  //SET WEL 
    hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_PageProgram);      //发送写页命令   
  hal_w25q_spi_txrx((uint8_t)((WriteAddr)>>16) & 0xff); //发送24bit地址    
  hal_w25q_spi_txrx((uint8_t)((WriteAddr)>>8) & 0xff);   
  hal_w25q_spi_txrx((uint8_t)WriteAddr & 0xff);   
  for(i=0;i<NumByteToWrite;i++)hal_w25q_spi_txrx(pBuffer[i]);//循环写数  
    hal_w25q_spi_release();                            //取消片选 
    SPI_Flash_Wait_Busy();                     //等待写入结束
    __enable_irq();      
} 

void SPI_Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{                    
    uint16_t pageremain;    
    __disable_irq();  

    pageremain=256-WriteAddr%256; //单页剩余的字节数                
    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
    while(1)
    {      
        SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)break;//写入结束了
        else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;  

            NumByteToWrite-=pageremain;           //减去已经写入了的字节数
            if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
            else pageremain=NumByteToWrite;       //不够256个字节了
        }
    };     
    __enable_irq();       
} 
 

uint8_t SPI_FLASH_BUF[4096];
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;    
    uint16_t i;    
    __disable_irq();  
    secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
    secoff=WriteAddr%4096;//在扇区内的偏移
    secremain=4096-secoff;//扇区剩余空间大小   

    if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
    while(1) 
    {   
        SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
        for(i=0;i<secremain;i++)//校验数据
        {
            if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除     
        }
        if(i<secremain)//需要擦除
        {
            SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
            for(i=0;i<secremain;i++)       //复制
            {
                SPI_FLASH_BUF[i+secoff]=pBuffer[i];   
            }
            SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区  

        }else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.                   
        if(NumByteToWrite==secremain)break;//写入结束了
        else//写入未结束
        {
            secpos++;//扇区地址增1
            secoff=0;//偏移位置为0    

            pBuffer+=secremain;  //指针偏移
            WriteAddr+=secremain;//写地址偏移       
            NumByteToWrite-=secremain;              //字节数递减
            if(NumByteToWrite>4096)secremain=4096;  //下一个扇区还是写不完
            else secremain=NumByteToWrite;          //下一个扇区可以写完了
        }    
    };      
    __enable_irq();       
}
 

void SPI_Flash_Erase_Chip(void)   
{                                             
  SPI_FLASH_Write_Enable();                  //SET WEL 
  SPI_Flash_Wait_Busy();   
  hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_ChipErase);        //发送片擦除命令  
  hal_w25q_spi_release();                            //取消片选             
  SPI_Flash_Wait_Busy();                     //等待芯片擦除结束
}   



void SPI_Flash_Erase_Sector(uint32_t Dst_Addr)   
{   
    Dst_Addr*=4096;
  __disable_irq();  
  SPI_FLASH_Write_Enable();                  //SET WEL   
  SPI_Flash_Wait_Busy();   
  hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_SectorErase);      //发送扇区擦除指令 
  hal_w25q_spi_txrx((uint8_t)((Dst_Addr)>>16) & 0xff);  //发送24bit地址    
  hal_w25q_spi_txrx((uint8_t)((Dst_Addr)>>8) & 0xff);   
  hal_w25q_spi_txrx((uint8_t)Dst_Addr & 0xff);  
  hal_w25q_spi_release();                            //取消片选             
  SPI_Flash_Wait_Busy();                   //等待擦除完成
  __enable_irq();      
}  


void SPI_Flash_Wait_Busy(void)   
{   
    while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  

void SPI_Flash_PowerDown(void)   
{ 
  hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_PowerDown);        //发送掉电命令  
  hal_w25q_spi_release();                            //取消片选             
  delay(3);                               //等待TPD  
}   
//唤醒
void SPI_Flash_WAKEUP(void)   
{  
  hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
  hal_w25q_spi_release();                            //取消片选             
  delay(3);                               //等待TRES1
}   





flash_info_t* flash_spi_getcardinfo(void){
    return &flashinfo;
}






static unsigned char disk_read4K (uint8_t *rxbuf, uint32_t sector, uint32_t count){
  unsigned char res=0;
  for(;count>0;count--)
  {
    SPI_Flash_Read(rxbuf,sector*FLASH_SECTOR_SIZE4K,FLASH_SECTOR_SIZE4K);
    sector++;
    rxbuf+=FLASH_SECTOR_SIZE4K;
  }
  hw_toggle_pin(GPIOx(GPIO_C),13);
  return res;
}
static unsigned char disk_write4K (const uint8_t *txbuf, uint32_t sector, uint32_t count){
  unsigned char res=0;
  for(;count>0;count--)
  {                       
     SPI_Flash_Write((uint8_t*)txbuf,sector*FLASH_SECTOR_SIZE4K,FLASH_SECTOR_SIZE4K);
     sector++;
     txbuf+=FLASH_SECTOR_SIZE4K;
  }
  hw_toggle_pin(GPIOx(GPIO_C),13);
  return res;
}


//////////////////////////////

static unsigned char disk_read (uint8_t *rxbuf, uint32_t sector, uint32_t count){
  unsigned char res=0;
  for(;count>0;count--)
  {
    SPI_Flash_Read(rxbuf,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
    sector++;
    rxbuf+=FLASH_SECTOR_SIZE;
  }
  hw_toggle_pin(GPIOx(GPIO_C),13);
  return res;
}
static unsigned char disk_write (const uint8_t *txbuf, uint32_t sector, uint32_t count){
  unsigned char res=0;
  for(;count>0;count--)
  {                       
     SPI_Flash_Write((uint8_t*)txbuf,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
     sector++;
     txbuf+=FLASH_SECTOR_SIZE;
  }
  hw_toggle_pin(GPIOx(GPIO_C),13);
  return res;
}


const w25qxxx_drv_t4K w25qxxx_drv4K =
{
    disk_read4K,//sd_spi_read,
    disk_write4K,//sd_spi_write,
};


const w25qxxx_drv_t w25qxxx_drv =
{
    SPI_Flash_Init,
    disk_read,//sd_spi_read,
    disk_write,//sd_spi_write,
    flash_spi_getcardinfo,//sd_spi_getcardinfo
};



 
/*

uint8_t lowmemRAM[BLOCK];
uint8_t cacheRAM1[BLOCK];
uint8_t cacheRAM2[BLOCK];
//uint8_t cacheRAM3[BLOCK];
//uint8_t cacheRAM4[BLOCK];

uint32_t block1=0;
uint32_t block2=0;
//uint32_t block3=0;
//uint32_t block4=0;

uint8_t dirty1=0;
uint8_t dirty2=0;
//uint8_t dirty3=0;
//uint8_t dirty4=0;

uint8_t NextCacheUse=0;
uint32_t max=0;

//static const uint32_t base = 4000000;
uint32_t sector = 0 ;/// BLOCK;

void cachebegin(uint32_t size){
   max=size;
   block1=sector;
   block2=sector+1;
   //block3=sector+2;
   //block4=sector+3;

   dirty1=0;
   dirty2=0;
   //dirty3=0;
   //dirty4=0;
 
   NextCacheUse=0;
   memset(lowmemRAM,0,sizeof(lowmemRAM));
   memset(cacheRAM1,0,BLOCK);
   memset(cacheRAM2,0,BLOCK);

   for(int i=0;i<(size/BLOCK)+1;i++){
       //kprintf("%d %d\n",i,sector+i);
   }
   //disk_initialize(0);
   //printf("\nblocks %d %d %d %d\n",block1,block2,block3,block4 );
   kprintf("size memory: %d, sector: %d\n",max,sector );
}

uint32_t cacheget_size(){
       return max;
}



void cachewrite(uint32_t address,uint8_t value){
    if (address<sizeof(lowmemRAM)) {
        lowmemRAM[address]=value; //If RAM lower than 0x600 write direct RAM
        return;
    }
    uint32_t block=(address&0xfffff000); //Else write into cached block
    
    if (block1==block) {
      cacheRAM1[address&0x00000fff]=value; //Write in cache1
      dirty1=1; //mark cache1 as dirty
      return; 
    }
    
    if (block2==block) {
      cacheRAM2[address&0x00000fff]=value; //Write in cache2
      dirty2=1; //mark cache2 as dirty
      return; 
    }
    
    if (NextCacheUse==0) {
      if (dirty1) {           //Write back the cached block if it's dirty
        disk_write ((cacheRAM1), block1, 1);
      }
      
      disk_read ( (cacheRAM1), block, 1);
      block1=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      cacheRAM1[address&0x00000fff]=value; //Now write it in cache1
      dirty1=1;
      return;
    }
    
    if (NextCacheUse==1) {
      if (dirty2) {           //Write back the cached block if it's dirty
          disk_write ((cacheRAM2), block2, 1);
      }
      disk_read ( (cacheRAM2), block, 1);
      block2=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      cacheRAM2[address&0x00000fff]=value; //Now write it in cache2
      dirty2=1;
      return;
    }
}

uint8_t cacheread(uint32_t address){
    if (address<sizeof(lowmemRAM)) return lowmemRAM[address]; //If RAM lower than 0x600 return direct RAM
    uint32_t block=(address&0xfffff000); //Else look into cached RAM
    if (block1==block) return cacheRAM1[address&0x00000fff]; //Cache hit in cache1
    if (block2==block) return cacheRAM2[address&0x00000fff]; //Cache hit in cache2
    if (NextCacheUse==0) {
      if (dirty1) {           //Write back the cached block if it's dirty
        disk_write ((cacheRAM1), block1, 1);
        dirty1=0;
      }
      
      disk_read ( (cacheRAM1), block, 1);
      block1=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      return cacheRAM1[address&0x00000fff]; //Now found it in cache1
    }
    
    if (NextCacheUse==1) {
      if (dirty2) {           //Write back the cached block if it's dirty
        disk_write ( (cacheRAM2), block2, 1);
        dirty2=0;
      }
      disk_read ( (cacheRAM2), block, 1);
      block2=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      return cacheRAM2[address&0x00000fff]; //Now found it in cache2
    }
}*/