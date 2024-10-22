
#include <spi.h>
#include <gpio.h>



//--------------------------------------------
// GPIO_AF5_SPI1(APB2):
// SPI1 SCK:   PA5    PB3
// SPI1 MISO:  PA6    PB4
// SPI1 MOSI:  PA7    PB5
// GPIO_AF5_SPI2(APB1):
// SPI2 SCK:   PB10   PB13    PI0
// SPI2 MISO:  PC2    PB14    PI2
// SPI2 MOSI:  PC3    PB15    PI3
// GPIO_AF6_SPI3(APB1):
// SPI3 SCK:   PB3    PC10
// SPI3 MISO:  PB4    PC11
// SPI3 MOSI:  PB5    PC12

//--------------------------------------------
#define PORT_SCK        GPIO_A   // PB3 --> SPI1_SCK
#define PIN_SCK         5
#define PORT_MISO       GPIO_A   // PB4 <-- SPI1_MISO
#define PIN_MISO        6
#define PORT_MOSI       GPIO_A   // PB5 --> SPI1_MOSI
#define PIN_MOSI        7
#define PORT_CS         GPIO_A   // PB14 --> F_CS(0)
#define PIN_CS          4
//cs a15
//miso b4
//sck b3
//mosi b5

#define SPI_CLK_DIV      SPI_CR1_BR_2
// SPI Initialization Frequency (400kHz max)
// SPI2_CK = PCLK1(54MHz) / 256 = 210.9375kHz
#define SPI_INIT_CLK_DIV          (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)
// SPI Data Transfer Frequency (25MHz max)
// SPI2_CK = PCLK1(54MHz) / 4 = 13.5MHz
#define SPI_TRANSFER_CLK_DIV      SPI_CR1_BR_0

//--------------------------------------------
void hal_w25q_spi_init(void)
{
    // SPI1 clock enable
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // IO port A clock enable
    // IO port B clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

    hw_cfg_pin(GPIOx(PORT_SCK),    PIN_SCK,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_MISO),   PIN_MISO,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_MOSI),   PIN_MOSI,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_CS),     PIN_CS,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

    // BIDIMODE = 0: 2-line unidirectional data mode selected
    // BIDIOE = 0: irrelevant
    // SSM = 1: Software slave management enabled
    // SSI = 1: NSS pin is ignored
    // SPE = 1: SPI enable
    // BR[2:0]: Baud rate control
    // MSTR = 1: Master configuration
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CLK_DIV | SPI_CR1_MSTR;
    
    hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_TRANSFER_CLK_DIV | SPI_CR1_MSTR;
}

//--------------------------------------------
void hal_w25q_spi_select(void)
{
    hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);
}

//--------------------------------------------
void hal_w25q_spi_release(void)
{
    hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);
}

//--------------------------------------------
uint8_t hal_w25q_spi_txrx(uint8_t data)
{
    
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
    while (!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}

//--------------------------------------------
#if 0
void hal_w25q_spi_slow(void)
{
    // BIDIMODE = 0: 2-line unidirectional data mode selected
    // BIDIOE = 0: irrelevant
    // SSM = 1: Software slave management enabled
    // SSI = 1: NSS pin is ignored
    // SPE = 1: SPI enable
    // BR[2:0]: Baud rate control
    // MSTR = 1: Master configuration
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_INIT_CLK_DIV | SPI_CR1_MSTR;

    // FRXTH = 1: RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
    SPI1->CR2 = SPI_CR2_FRXTH;
}

//--------------------------------------------
void hal_w25q_spi_fast(void)
{
    // BIDIMODE = 0: 2-line unidirectional data mode selected
    // BIDIOE = 0: irrelevant
    // SSM = 1: Software slave management enabled
    // SSI = 1: NSS pin is ignored
    // SPE = 1: SPI enable
    // BR[2:0]: Baud rate control
    // MSTR = 1: Master configuration
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_TRANSFER_CLK_DIV | SPI_CR1_MSTR;

    // FRXTH = 1: RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
    SPI1->CR2 = SPI_CR2_FRXTH;
}



#endif

#define PORT_SCK2        GPIO_B   // PB3 --> SPI1_SCK
#define PIN_SCK2         13
#define PORT_MISO2       GPIO_B   // PB4 <-- SPI1_MISO
#define PIN_MISO2        14
#define PORT_MOSI2       GPIO_B   // PB5 --> SPI1_MOSI
#define PIN_MOSI2        15
#define PORT_CS2         GPIO_B   // PB14 --> F_CS(0)
#define PIN_CS2          12


//--------------------------------------------
void hal_sd_spi_init(void)
{
    // SPI1 clock enable
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    // IO port A clock enable
    // IO port B clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    hw_cfg_pin(GPIOx(PORT_SCK2),    PIN_SCK2,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_MISO2),   PIN_MISO2,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_MOSI2),   PIN_MOSI2,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_CS2),     PIN_CS2,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

    // BIDIMODE = 0: 2-line unidirectional data mode selected
    // BIDIOE = 0: irrelevant
    // SSM = 1: Software slave management enabled
    // SSI = 1: NSS pin is ignored
    // SPE = 1: SPI enable
    // BR[2:0]: Baud rate control
    // MSTR = 1: Master configuration
    SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR_2 | SPI_CR1_MSTR;

    hw_set_pin(GPIOx(PORT_CS2), PIN_CS2, 1);
    ///SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_TRANSFER_CLK_DIV | SPI_CR1_MSTR;
}

//--------------------------------------------
void hal_sd_spi_select(void)
{
    hw_set_pin(GPIOx(PORT_CS2), PIN_CS2, 0);
}

//--------------------------------------------
void hal_sd_spi_release(void)
{
    hw_set_pin(GPIOx(PORT_CS2), PIN_CS2, 1);
}

//--------------------------------------------
uint8_t hal_sd_spi_txrx(uint8_t data)
{
    while (!(SPI2->SR & SPI_SR_TXE));
    SPI2->DR = data;
    while (!(SPI2->SR & SPI_SR_RXNE));
    return SPI2->DR;
}


void hal_sd_spi_slow(void)
{
    // BIDIMODE = 0: 2-line unidirectional data mode selected
    // BIDIOE = 0: irrelevant
    // SSM = 1: Software slave management enabled
    // SSI = 1: NSS pin is ignored
    // SPE = 1: SPI enable
    // BR[2:0]: Baud rate control
    // MSTR = 1: Master configuration
    SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_INIT_CLK_DIV | SPI_CR1_MSTR;
}

//--------------------------------------------
void hal_sd_spi_fast(void)
{
    // BIDIMODE = 0: 2-line unidirectional data mode selected
    // BIDIOE = 0: irrelevant
    // SSM = 1: Software slave management enabled
    // SSI = 1: NSS pin is ignored
    // SPE = 1: SPI enable
    // BR[2:0]: Baud rate control
    // MSTR = 1: Master configuration
    SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_TRANSFER_CLK_DIV | SPI_CR1_MSTR;
}

