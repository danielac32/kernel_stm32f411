#ifndef __RISCV_H__
#define __RISCV_H__

#include <queue.h>

#define MTIM 7
#define MSIT 3
#define MEIT 11
#define timel *(unsigned int*)0x11004000
#define timeh *(unsigned int*)0x11004004
#define kbhit() *(uint8*)0x10000005
#define readbyte() *(uint8*)0x10000000
extern uint32_t SYSCON;
extern uint32_t reset;
extern void enable_interrupt_status();
extern void enable_interrupt(unsigned int num);
extern void disable_interrupt_status();
extern void disable_interrupt(unsigned int num);
extern void clear_interrupts();
extern void soft_trap();
 
#define UART *(char *)0x10000000L

extern void enable(void);
extern uint32_t disable(void);
extern uint32_t restore(uint32_t irq);

// which hart (core) is this?


#define MSTATUS_MPIE        0x00000080
// Machine Status Register, mstatus
#define MSTATUS_MPP_MASK (3L << 11) // previous mode.
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MIE (1L << 3) // machine-mode interrupt enable.

// Machine-mode Interrupt Enable
#define MIE_MEIE (1L << 11) // external
#define MIE_MTIE (1L << 7)  // timer
#define MIE_MSIE (1L << 3)  // software



static inline void timer_trap(uint32_t x)
{
  asm volatile("csrw 0x403, %0"
               :
               : "r"(x));
}
static inline void csr_put(uint32_t c)
{
  asm volatile("csrw 0x402, %0"
               :
               : "r"(c));
}
static inline void upload_fs(uint32_t x)
{
  asm volatile("csrw 0x401, %0"
               :
               : "r"(x));
}
 
 
static inline void download_fs(uint32_t x)
{
  asm volatile("csrw 0x400, %0"
               :
               : "r"(x));
}
static inline uint32_t r_mhartid()
{
  uint32_t x;
  asm volatile("csrr %0, mhartid"
               : "=r"(x));
  return x;
}

static inline uint32_t r_mstatus()
{
  uint32_t x;
  asm volatile("csrr %0, mstatus"
               : "=r"(x));
  return x;
}

static inline void w_mstatus(uint32_t x)
{
  asm volatile("csrw mstatus, %0"
               :
               : "r"(x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_mepc(uint32_t x)
{
  asm volatile("csrw mepc, %0"
               :
               : "r"(x));
}

static inline uint32_t r_mepc()
{
  uint32_t x;
  asm volatile("csrr %0, mepc"
               : "=r"(x));
  return x;
}

// Machine Scratch register, for early trap handler
static inline void w_mscratch(uint32_t x)
{
  asm volatile("csrw mscratch, %0"
               :
               : "r"(x));
}

// Machine-mode interrupt vector
static inline void w_mtvec(uint32_t x)
{
  asm volatile("csrw mtvec, %0"
               :
               : "r"(x));
}

static inline uint32_t csrr()
{
  uint32_t x;
  asm volatile("csrr %0, mie"
               : "=r"(x));
  return x;
}

static inline uint32_t r_mie()
{
  uint32_t x;
  asm volatile("csrr %0, mie"
               : "=r"(x));
  return x;
}

static inline uint32_t r_mip()
{
  uint32_t x;
  asm volatile("csrr %0, mip"
               : "=r"(x));
  return x;
}


static inline void w_mie(uint32_t x)
{
  asm volatile("csrw mie, %0"
               :
               : "r"(x));
}

static inline void w_mip(uint32_t x)
{
  asm volatile("csrw mip, %0"
               :
               : "r"(x));
}


static inline uint32_t get_cyc_count() {
  uint32_t ccount;
  asm volatile("csrr %0, 0xC00":"=r" (ccount));
  return ccount;
}

#define csr_read(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define csr_write(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define csr_set(reg, bit) ({ unsigned long __tmp; \
  asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
  __tmp; })

#define csr_clear(reg, bit) ({ unsigned long __tmp; \
  asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
  __tmp; })

#define csr_swap(reg, val) ({ \
    unsigned long __v = (unsigned long)(val); \
    asm volatile ("csrrw %0, " #reg ", %1" : "=r" (__v) : "rK" (__v) : "memory"); \
    __v; })



#define REG_RA                         1
#define REG_SP                         2
#define REG_ARG0                       10
#define REG_RET                        REG_ARG0
#define NUM_GP_REG                     32
#define NUM_CSR_REG                    3

#define CAUSE_MISALIGNED_FETCH         0
#define CAUSE_FAULT_FETCH              1
#define CAUSE_ILLEGAL_INSTRUCTION      2
#define CAUSE_BREAKPOINT               3
#define CAUSE_MISALIGNED_LOAD          4
#define CAUSE_FAULT_LOAD               5
#define CAUSE_MISALIGNED_STORE         6
#define CAUSE_FAULT_STORE              7
#define CAUSE_ECALL_U                  8
#define CAUSE_ECALL_S                  9
#define CAUSE_ECALL_M                  11
#define CAUSE_PAGE_FAULT_INST          12
#define CAUSE_PAGE_FAULT_LOAD          13
#define CAUSE_PAGE_FAULT_STORE         15
#define CAUSE_INTERRUPT                (1 << 31)


static inline uint32_t csr_get_time(void)
{
    uint32_t value;
    asm volatile ("csrr %0, time" : "=r" (value) : );
    return value;
}

struct irq_context
{
    uint32_t pc;
    uint32_t status;
    uint32_t cause;
    uint32_t reg[NUM_GP_REG];
};
void    exception_dump_ctx(struct irq_context *ctx);
struct irq_context * exceptionHandler(struct irq_context *ctx);
struct irq_context *do_syscall(struct irq_context *ctx);




static inline __attribute__((always_inline)) unsigned int irq_enable(void)
{
    /* Enable all interrupts */
    unsigned state;

    __asm__ volatile (
        "csrrs %[dest], mstatus, %[mask]"
        :[dest]    "=r" (state)
        :[mask]    "i" (MSTATUS_MIE)
        : "memory"
        );
    return state;
}


static inline __attribute__((always_inline)) unsigned int irq_disable(void)
{

    unsigned int state;

    __asm__ volatile (
        "csrrc %[dest], mstatus, %[mask]"
        :[dest]    "=r" (state)
        :[mask]    "i" (MSTATUS_MIE)
        : "memory"
        );

    return state;
}


static inline __attribute__((always_inline)) void irq_restore(
    unsigned int state)
{
    /* Restore all interrupts to given state */
    __asm__ volatile (
        "csrw mstatus, %[state]"
        : /* no outputs */
        :[state]   "r" (state)
        : "memory"
        );
}

extern sem_t uart_tx;
extern sem_t uart_rx;




/***********************************************************/




#endif
