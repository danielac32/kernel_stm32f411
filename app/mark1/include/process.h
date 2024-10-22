#ifndef PROCESS_H
#define PROCESS_H

  
 
/* unusual value marks the top of the thread stack                      */
#define STACKMAGIC  0x0A0AAAA9

/* thread state constants                                               */
#define PR_FREE   0 /* Process table entry is unused  */
#define PR_CURR   1 /* Process is currently running   */
#define PR_READY  2 /* Process is on ready queue    */
#define PR_RECV   3 /* Process waiting for message    */
#define PR_SLEEP  4 /* Process is sleeping      */
#define PR_SUSP   5 /* Process is suspended     */
#define PR_WAIT   6 /* Process is on semaphore queue  */
#define PR_RECTIM 7 /* Process is receiving with timeout  */
#define PR_KILL     8


/* miscellaneous thread definitions                                     */
#define TNMLEN      16          /**< length of thread "name"            */
#define NULLTHREAD  0           /**< id of the null thread              */
#define BADTID      (-1)        /**< used when invalid tid needed       */

#define INITSTK     4096 

/* Reschedule constants for ready  */
#define RESCHED_YES 1           /**< tell ready to reschedule           */
#define RESCHED_NO  0           /**< tell ready not to reschedule       */
#define NTHREAD   100        


typedef struct task_t
{
    uint8 state;                /**< thread state: THRCURR, etc.        */
    int prio;                   /**< thread priority                    */
    void *stkptr;               /**< saved stack pointer                */
    void *stkbase;              /**< base of run time stack             */
    uint32 stklen;               /**< stack length in bytes              */
    char name[TNMLEN];          /**< thread name                        */
    uint32 param;
    int pid;
    //irqmask intmask;            /**< saved interrupt mask               */
    //semaphore sem;              /**< semaphore waiting for              */
   // tid_typ parent;             /**< tid for the parent thread          */
    //message msg;                /**< message sent to this thread        */
    //bool hasmsg;                /**< nonzero iff msg is valid           */
    //struct memblock memlist;    /**< free memory list of thread         */
    //int fdesc[NDESC];           /**< device descriptors for thread      */
    bool8   elf;
    void *img;
    
    struct task_t *next;

}task_t;


#define isbadtid(x) ((x)>=NTHREAD || (x)<0 || PR_FREE == thrtab[(x)].state)

extern uint32 preempt;
extern task_t thrtab[];
extern int thrcount;            /**< currently active threads           */
extern int thrcurrent;      /**< currently executing thread         */
extern task_t *active_task;
extern uint32 sys_time;
int create(void *procaddr, uint32 ssize, int priority,
               const char *name, int nargs, ...);
int getpid();
int getprio(int);
void kill(int);
int ready(int/*, bool*/);
void resched(void);
void sleep(uint32 ticks);
int yield(void);
void userret(void);
void clkhandler(void);

extern void ctxsw(void *, void *, uint8);


#endif