
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb
.text

.globl	entry
entry:
b main
loop:
b loop


.globl	__syscall
__syscall:
svc 0
nop
bx	lr


.text
    .globl  irq_disable
    .globl  irq_restore
    .globl  irq_enable

/*------------------------------------------------------------------------
 * disable  -  Disable interrupts and return the previous state
 *------------------------------------------------------------------------
 */
irq_disable:
    mrs r0, psr     /* Copy the CPSR into r0        */
    cpsid   i       /* Disable interrupts           */
    bx  lr      /* Return the CPSR          */

/*------------------------------------------------------------------------
 * restore  -  Restore interrupts to value given by mask argument
   Cortex M3 hardware handles a lot, rewrite
 *------------------------------------------------------------------------
 */
irq_restore:
    msr psr_nzcvq, r0   /* Restore the CPSR         */
    cpsie   i       
    bx  lr      /* Return to caller         */

/*------------------------------------------------------------------------
 * enable  -  Enable interrupts
 *------------------------------------------------------------------------
 */
irq_enable:
    cpsie   i       /* Enable interrupts            */
    bx  lr      /* Return               */
