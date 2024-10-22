#include <xinu.h>
#include <gpio.h>


devcall	setLED( struct dentry *devptr, char c){

	hw_set_pin(GPIOx(GPIO_C), 13, c);
	return OK;
}