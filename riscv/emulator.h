#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <stdint.h>
//#include <w25qxxx.h>


enum emulatorCode{
    EMU_POWEROFF,
    EMU_REBOOT,
    EMU_UNKNOWN
};

int riscv_emu();
int load_sd_file( uint32_t part, const char filename[] );
extern void SPI_Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
extern void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);

//#define base 2*1024*1024
//#define psram_read(addr, buf, len) SPI_Flash_Read(buf,addr+base,len)
//#define psram_write(addr, buf, len) SPI_Flash_Write(buf,addr+base,len)

#endif