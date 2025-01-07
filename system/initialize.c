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

/* initialize.c - nulluser, sysinit */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <gpio.h>
#include <usb_cdc_conf.h>
#include "disk.h"
#include "fat_filelib.h"
#include <syscall.h>
#include <w25qxxx.h>
syscall_t syscallp;
extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/

/* Function prototypes */

extern	void main(void);	/* Main is the first process created	*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/
extern int ready_preemptive;
//extern void usbTask();
/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/
//bool start_null=false;
/* Control sequence to reset the console colors and cusor positiion	*/

#define	CONSOLE_RESET	" \033[0m\033[2J\033[;H"
 


int blink2(int nargs, char *args[])
{
	//resume(create(blink, 1024, 50, "blink", 0));
    for(int i=0;i<nargs;i++){
    	printf("%s\n",args[i]);
    }
	printf("blink2\n");
	return OK;
}


void blink1(){
	int c=0;
	int ulen;
	char	buf[32];
    printf("blink1\n");
	resume(create(blink2, INITSTK, 60, "blink2", 3,"daniel","quintero","kernel"));
	receive();
    
    printf("\nprocess has completed.\n");
	while(1){
        //printf("nullp %d\n",c++);
        sleep(3);
        /*while(!usb_available()){
              PEND_SV();
        }
        while(usb_available()){
              ulen = readBytes(buf,sizeof(buf));
        }
        buf[ulen] = SH_NEWLINE;
        kprintf("->%s\n",buf);*/
	}
}

int  initFat32(){
	uint32 size=sd_init();
    fl_init();
      // Attach media access functions to library
	if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK)
	{
	      printf("ERROR: Failed to init file system\n");
	      return -1;
	}
    //printf("fat32 (%d) %d\n",512,size);
  // List the root directory
    //fl_listdirectory("/");
    return OK;
}

/*
int usbTask(){
    while(1){
       if(usb_available()){
            // printf("aqui usb\n" );
            //kputc(usb_getc());
            //uint32 q = disable();
            ttyhandler(1,usb_getc(),0);
            //restore(q);
        }
        //printf("usbTask\n");
    }
    return 0;
}
*/



uint32 idleTicks;





char *getUrlTargetFileBoot(){
    //char *s=full_path("/config/kernel");
    FILE *fptr;
    char buff[64];
    int i=0;
    if ((fptr = fopen("/config/kernel","r")) == NULL){
        printf("Error! opening file\n");
        printf("jump app\n");
        return NULL;
    }
    fseek(fptr, 0, SEEK_END);
    int lsize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char *b=&buff;// malloc(lsize+1);

    while(!feof(fptr)){
         buff[i]=fgetc(fptr);
         i++;
    }
    //fread(b,lsize,1, fptr);
    //b[lsize]=0;
    //b[lsize+1]=0;
    //kprintf("%s %s\n",t,b);
    fclose(fptr);
    return (char *)b;
}



/*
int nullprocess(void) {
	//ready(create(usbTask, 2048, 1, "usbtask", 0));
	 
    //resume(create(blink1, INITSTK, 50, "blink1", 0));
  syscall_init(&syscallp);
  //syscallp.putc(CONSOLE,'a');
  //syscallp.putc(CONSOLE,'b');
  syscallp.puts(CONSOLE,"syscall init\n",13);
  //ready(create(usbTask, 1024, 1, "usbtask", 0));
  
    

	resume(create(shell, 4096, 52, "shell", 1, CONSOLE));
    resume(create(initFat32, 1024, 50, "fat32", 0));
	//resume(create(usbTask, 2048, 1, "usbtask", 0));
	//resume(create(blink, 1024/2, 51, "blink", 0));
    //resume(create(blink1, INITSTK, 50, "blink1", 0));
	//for(;;){

	//}
   
    while(1){
    	idleTicks++;
    }
    //load_sd_file( 0, "kernel.bin" );
	return OK;
}

*/
 

int start_process(){
  int32	msg;
  int pid = create(initFat32, 1024, 50, "fat32", 0);
  msg = recvclr();
	resume(pid);
  msg = receive();
	while (msg != pid) {
		msg = receive();
	}
	ready(create(shell, 4096, 52, "shell", 1, 0));

    printf("boot: %s\n",getUrlTargetFileBoot() );

	return 0;
}


int nullprocess(void) {

     syscall_init(&syscallp);
	 resume(create(start_process, 4096, 50, "start", 1, 0));
	 while(1);
	 return 0;
}



void	nulluser()
{	
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/

    hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);

    meminit();
	platinit();
    /* Enable interrupts */
	enable();

	/* Initialize the system */
   
    while(1){
        if(!hw_get_pin(GPIOx(GPIO_A),0) || usb_available()){
            break;
        }
        hw_toggle_pin(GPIOx(GPIO_C),13);
        delay(50);
    }
	
    sysinit();
	/* Output Xinu memory layout */
	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	kprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);
	kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
	for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
	    kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
	}

	kprintf("%10d bytes of Xinu code.\n",
		(uint32)&_etext - (uint32)&_text);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)&_text, (uint32)&_etext - 1);
	kprintf("%10d bytes of data.\n",
		(uint32)&_ebss - (uint32)&_sdata);
	kprintf("           [0x%08X to 0x%08X]\n\n",
		(uint32)&_sdata, (uint32)&_ebss - 1);

     
	/* Initialize the Null process entry */	
	int pid = create((void *)nullprocess, 1024, 10, "Null process", 0, NULL);
	struct procent * prptr = &proctab[pid];
	prptr->prstate = PR_CURR;
 
	
	/* Initialize the real time clock */
	clkinit();

    TIM2->CR1 |= (1 << 0); 
    ready_preemptive=1;
    NVIC_EnableIRQ(TIM2_IRQn);
    //__syscall(XINU_NULLPROCESS,prptr);


    asm volatile ("mov r0, %0\n" : : "r" (prptr->prstkptr));
	asm volatile ("msr psp, r0");
	asm volatile ("ldmia r0!, {r4-r11} ");
    asm volatile ("msr psp, r0");
	asm volatile ("mov r0, #2");
	asm volatile ("msr control, r0");
	asm volatile ("isb");
    nullprocess();
     //PEND_SV();

	for(;;);

}

/* Startup does a system call, the processor switches to handler 
 * mode and prepares for executing the null process (see syscall.c) 
 * This is also where a kernel mode to user mode switch can
 * take place */


/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */


 
static	void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/


	/* Platform Specific Initialization */
    //meminit();
	  //platinit();

	//kprintf(CONSOLE_RESET);
	for (int i = 0; i < 10; ++i)
	{
		kprintf("\n");
	}
	/* Initialize free memory list */
	
	

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */
	prcount = 0;
//	prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
	}


	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}

	
	readylist = newqueue();

	
	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}


 
int32	stop(char *s)
{
	kprintf("%s\n", s);
	kprintf("looping... press reset\n");
	while(1)
	;
}

 
 