/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */

/* platinit.c - platinit */

#include <stm32.h>
#include <usb_cdc_conf.h>
#include <spi.h>
#include <w25qxxx.h>
#include <interrupt.h>
/*------------------------------------------------------------------------
 * platinit - platform specific initialization
 *------------------------------------------------------------------------
 */
void platinit(void)
{
	//hal_w25q_spi_init();
    //SPI_Flash_Init();
    RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
    _BST(GPIOA->AFR[1], (0x0A << 12) | (0x0A << 16));
    _BMD(GPIOA->MODER, (0x03 << 22) | (0x03 << 24), (0x02 << 22) | (0x02 << 24));
    cdc_init_usbd();
    NVIC_EnableIRQ(OTG_FS_IRQn);
    usbd_enable(&udev, true);
    usbd_connect(&udev, true);
}

 