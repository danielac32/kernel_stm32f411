
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
/* GPIO Bank Interface ********************************************************/
 
 
// GPIOCFG macros
#define GPIO_AF_BITS            4       // width of bit field
#define GPIO_AF_MASK            0x000F  // mask in AFR[0/1]
#define GPIO_AF0_MCO            0
#define GPIO_AF1_TIM1           1
#define GPIO_AF4_I2C1           4
#define GPIO_AF4_I2C2           4
#define GPIO_AF4_I2C3           4
#define GPIO_AF5_SPI1           5
#define GPIO_AF5_SPI2           5
#define GPIO_AF6_SPI3           6
#define GPIO_AF7_USART1         7
#define GPIO_AF11_ETH           11
#define GPIO_AF12_FSMC          12
#define GPIO_AF12_SDIO          12
#define GPIO_AF13_DCMI          13


#define PIN_AF_RTC_50HZ         PIN_AF0
#define PIN_AF_MC0              PIN_AF0
#define PIN_AF_TAMPER           PIN_AF0
#define PIN_AF_SWJ              PIN_AF0
#define PIN_AF_TRACE            PIN_AF0

#define PIN_AF_TIM1             PIN_AF1
#define PIN_AF_TIM2             PIN_AF1

#define PIN_AF_TIM3             PIN_AF2
#define PIN_AF_TIM4             PIN_AF2
#define PIN_AF_TIM5             PIN_AF2

#define PIN_AF_TIM8             PIN_AF3
#define PIN_AF_TIM9             PIN_AF3
#define PIN_AF_TIM10            PIN_AF3
#define PIN_AF_TIM11            PIN_AF3

#define PIN_AF_I2C1             PIN_AF4
#define PIN_AF_I2C2             PIN_AF4
#define PIN_AF_I2C3             PIN_AF4
/* Since all I2C use the same PIN_AFn, generic name for convenience. */
#define PIN_AF_I2C              PIN_AF4

#define PIN_AF_SPI1             PIN_AF5
#define PIN_AF_SPI2             PIN_AF5

#define PIN_AF_SPI3             PIN_AF6

#define PIN_AF_USART1           PIN_AF7
#define PIN_AF_USART2           PIN_AF7
#define PIN_AF_USART3           PIN_AF7
#define PIN_AF_I2S3ext          PIN_AF7

#define PIN_AF_UART4            PIN_AF8
#define PIN_AF_UART5            PIN_AF8
#define PIN_AF_USART6           PIN_AF8

#define PIN_AF_CAN1             PIN_AF9
#define PIN_AF_CAN2             PIN_AF9
#define PIN_AF_TIM12            PIN_AF9
#define PIN_AF_TIM13            PIN_AF9
#define PIN_AF_TIM14            PIN_AF9

#define PIN_AF_OTG_FS           PIN_AF10
#define PIN_AF_OTG_HS           PIN_AF10

#define PIN_AF_ETH              PIN_AF11

#define PIN_AF_FSMC             PIN_AF12
#define PIN_AF_OTG_HS_FS        PIN_AF12
#define PIN_AF_SDIO             PIN_AF12

#define PIN_AF_DCMI             PIN_AF13

#define PIN_AF_EVENTOUT         PIN_AF15

 

// GPIO by port number (A=0, B=1, ..)
#define GPIOx(no)               ((GPIO_TypeDef*)(GPIOA_BASE + (no) * (GPIOB_BASE-GPIOA_BASE)))
#define GPIO_A                  0
#define GPIO_B                  1
#define GPIO_C                  2
#define GPIO_D                  3
#define GPIO_E                  4
#define GPIO_F                  5
#define GPIO_G                  6

// GPIOCFG macros
#define GPIO_AF_BITS            4       // width of bit field
#define GPIO_AF_MASK            0x000F  // mask in AFR[0/1]
//#define GPIO_AF0_MCO            0
//#define GPIO_AF1_TIM1           1
//#define GPIO_AF4_I2C1           4
//#define GPIO_AF4_I2C2           4
//#define GPIO_AF4_I2C3           4
//#define GPIO_AF5_SPI1           5
//#define GPIO_AF5_SPI2           5
//#define GPIO_AF6_SPI3           6
//#define GPIO_AF7_USART1         7
#define GPIO_AF11_ETH           11
#define GPIO_AF12_FSMC          12
//#define GPIO_AF12_SDIO          12
#define GPIO_AF13_DCMI          13

#define GPIOCFG_MODE_SHIFT      4
#define GPIOCFG_MODE_MASK      (3 << GPIOCFG_MODE_SHIFT)
#define GPIOCFG_MODE_INP       (0 << GPIOCFG_MODE_SHIFT)    // 00: Input (reset state)
#define GPIOCFG_MODE_OUT       (1 << GPIOCFG_MODE_SHIFT)    // 01: General purpose output mode
#define GPIOCFG_MODE_ALT       (2 << GPIOCFG_MODE_SHIFT)    // 10: Alternate function mode
#define GPIOCFG_MODE_ANA       (3 << GPIOCFG_MODE_SHIFT)    // 11: Analog mode
#define GPIOCFG_OTYPE_SHIFT     6
#define GPIOCFG_OTYPE_MASK     (1 << GPIOCFG_OTYPE_SHIFT)
#define GPIOCFG_OTYPE_PUPD     (0 << GPIOCFG_OTYPE_SHIFT)   // 0: Output push-pull (reset state)
#define GPIOCFG_OTYPE_OPEN     (1 << GPIOCFG_OTYPE_SHIFT)   // 1: Output open-drain
#define GPIOCFG_OSPEED_SHIFT    7
#define GPIOCFG_OSPEED_MASK    (3 << GPIOCFG_OSPEED_SHIFT)
#define GPIOCFG_OSPEED_LOW     (0 << GPIOCFG_OSPEED_SHIFT)  // 00: Low speed       (   2 MHz)
#define GPIOCFG_OSPEED_MEDIUM  (1 << GPIOCFG_OSPEED_SHIFT)  // 01: Medium speed    (  25 MHz)
#define GPIOCFG_OSPEED_HIGH    (2 << GPIOCFG_OSPEED_SHIFT)  // 10: High speed      (  50 MHz)
#define GPIOCFG_OSPEED_VHIGH   (3 << GPIOCFG_OSPEED_SHIFT)  // 11: Very high speed ( 100 Mhz)
#define GPIOCFG_PUPD_SHIFT      9
#define GPIOCFG_PUPD_MASK      (3 << GPIOCFG_PUPD_SHIFT)
#define GPIOCFG_PUPD_NONE      (0 << GPIOCFG_PUPD_SHIFT)    // 00: No pull-up, pull-down
#define GPIOCFG_PUPD_PUP       (1 << GPIOCFG_PUPD_SHIFT)    // 01: Pull-up
#define GPIOCFG_PUPD_PDN       (2 << GPIOCFG_PUPD_SHIFT)    // 10: Pull-down
#define GPIOCFG_PUPD_RFU       (3 << GPIOCFG_PUPD_SHIFT)    // 11: Reserved

// IRQ triggers
#define GPIO_IRQ_MASK           0x38
#define GPIO_IRQ_FALLING        0x20
#define GPIO_IRQ_RISING         0x28
#define GPIO_IRQ_CHANGE         0x30

// configure operation mode of GPIO pin
void hw_cfg_pin(GPIO_TypeDef* gpioport, uint8 pin, uint16 gpiocfg);

// set state of GPIO output pin
void hw_set_pin(GPIO_TypeDef* gpioport, uint8 pin, uint8 state);

// get state of GPIO input pin
uint8_t hw_get_pin(GPIO_TypeDef* gpioport, uint8 pin);

void hw_toggle_pin(GPIO_TypeDef* gpioport, uint8 pin);
