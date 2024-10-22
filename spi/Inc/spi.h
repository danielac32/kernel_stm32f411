
#include <stdint.h>

#ifndef HAL_W25Q_SPI_H_
#define HAL_W25Q_SPI_H_

void hal_w25q_spi_init(void);
void hal_w25q_spi_select(void);
void hal_w25q_spi_release(void);
uint8_t hal_w25q_spi_txrx(uint8_t data);

void hal_sd_spi_init(void);
void hal_sd_spi_select(void);
void hal_sd_spi_release(void);
uint8_t hal_sd_spi_txrx(uint8_t data);
void hal_sd_spi_slow(void);
void hal_sd_spi_fast(void);




#endif // HAL_W25Q_SPI_H_