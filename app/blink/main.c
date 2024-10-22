
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <gpio.h>
#include <syscall.h>
 
syscall_t syscallp;




int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
   
   for (int i = 1; i < argc; ++i)
  {
    printf("%s ", argv[i]);
  }
  printf("\n");

    __syscall(XINU_PIN_MODE,GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    while(1){
     __syscall(XINU_PIN_SET,GPIOx(GPIO_C),13, 0);
       syscallp.sleep(100);
       __syscall(XINU_PIN_SET,GPIOx(GPIO_C),13, 1);
       syscallp.sleep(1000);
    }

	return 0;
}