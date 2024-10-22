#include <stm32.h>
#include <usb.h>
static usbd_device *devusb;

void usbd_hw_init(usbd_device *dev){
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
	_BST(GPIOA->AFR[1], (0x0A << 12) | (0x0A << 16));
    _BMD(GPIOA->MODER, (0x03 << 22) | (0x03 << 24), (0x02 << 22) | (0x02 << 24));
    
    //NVIC_SetPriority(OTG_FS_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 14, 0));
	//NVIC_EnableIRQ(OTG_FS_IRQn);
	devusb = dev;

}
/*
void OTG_FS_Handler(void)
{
	//usbd_poll(devusb);
}
*/