/* interrupt.h */
#include "stdint.h"
#include "stm32.h"
#include "kernel.h"
#define NVIC_STIR	(uint32 *) 0xE000EF00

/* System control block */
#define SCB_ICSR	(uint32 *) 0xE000ED04
#define PENDSV_INTR	28

/* System control block base */
//#define SCB_BASE	0xE000ED00

/* Faults enable */
#define	MPUFAULT_EN	16
#define	BUSFAULT_EN	17
#define USAGEFAULT_EN	18

/* Exception vector */
#define MEMFAULT_ENTRY		4
#define BUSFAULT_ENTRY		5
#define USAGEFAULT_ENTRY	6
#define SVC_ENTRY		11
#define PENDSV_ENTRY		14
#define SYSTICK_ENTRY		15

extern uint64_t clockticks; // rolls over after 2^64/96MHz = 6089.1097 years
extern uint64_t   cycleCount(void);
extern void delay(uint32_t usec);
//extern void delay_ms(uint32_t delay_ms);
extern void delay_us(uint32_t delay_us);
/* in file intr.S */
extern	intmask	disable(void);
extern	void	enable(void);
extern	void	restore(uint32);

#define SYS_ENTRY()   asm volatile("cpsid i")
#define SYS_EXIT()    asm volatile("cpsie i")