#include <stdint.h>
#include <kernel.h>
#include <interrupt.h>
#include <string.h>
#include <stm32.h>

extern uint32_t SystemCoreClock;
uint64_t clockticks = 0;//STK_LOAD_RELOAD + 1; // rolls over after 2^64/96MHz = 6089.1097 years

uint64_t   cycleCount(void) { 
	    return clockticks;// - (uint32_t)(SysTick->VAL & STK_LOAD_RELOAD); 
}

void   SysTick_Handler(void) { 
      clockticks  += 1;
     
}

void delay(uint32_t usec) {
     uint64_t now = cycleCount();
      //then = now + usec * clockspeed_hz / (usec/sec)
     uint64_t then = now + 1 * usec;
     while (cycleCount() < then)
         __NOP(); // wait
    
 }

//--------------------------------------------

//--------------------------------------------
void delay_us(uint32_t delay_us)
{
    uint32_t start;
    start = DWT->CYCCNT;
    delay_us *= (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < delay_us);
}