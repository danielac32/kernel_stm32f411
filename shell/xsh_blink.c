/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include <gpio.h>
/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
shellcmd xsh_blink(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/
    //hw_cfg_pin(GPIOx(GPIO_C),     13,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	while(1){
		set(LED,1);///hw_set_pin(GPIOx(GPIO_C), 13, 1);//pin_set(pin_c13);
		sleepms(500);
		set(LED,0);//hw_set_pin(GPIOx(GPIO_C), 13, 0);//pin_reset(pin_c13);
		sleep(1);
	}
	return 0;
}
