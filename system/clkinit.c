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

/* clkinit.c - clkinit STM32 */

#include <xinu.h>

uint32	clktime;		/* Seconds since boot			*/
uint32  count1000;              /* ms since last clock tick             */
qid16	sleepq;			/* Queue of sleeping processes		*/
uint32	preempt;		/* Preemption counter			*/

/*------------------------------------------------------------------------
 * clkinit  -  Initialize the clock and sleep queue at startup
 *------------------------------------------------------------------------
 */
void clkinit(void)
{
	
	 

	sleepq = newqueue();	/* Allocate a queue to hold the delta	*/
				/*   list of sleeping processes		*/
	preempt = QUANTUM;	/* Set the preemption time		*/
	clktime = 0;		/* Start counting seconds		*/
    count1000 = 0;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->CR1 |= (1 << 2);
    TIM2->PSC = 0x7;
    TIM2->ARR = 0x1000;
    TIM2->DIER |= 0x1;
    NVIC_DisableIRQ(TIM2_IRQn);
    //NVIC_EnableIRQ(TIM2_IRQn);
	return;
}
