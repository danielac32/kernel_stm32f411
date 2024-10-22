
//#include <stdio.h>



 


 extern shellcmd xsh_cpu(int nargs, char *args[]);
 
/* in file addargs.c */
extern	status	addargs(pid32, int32, int32[], int32,char *, void *);

 

/* in file chprio.c */
extern	pri16	chprio(pid32, pri16);

 

/* in file clkhandler.c */
//extern	interrupt clkhandler(void);
extern interrupt TIM2_Handler();

/* in file clkinit.c */
extern	void	clkinit(void);

/* in file clkint.S */
extern	void	clkint(void);

/* in file close.c */
extern	syscall	close(did32);

/* in file control.c */
extern	syscall	control(did32, int32, int32, int32);

/* in file create.c */
extern	pid32	create(void *, uint32, pri16, char *, uint32, ...);

/* in file ctxsw.S */
extern	void	ctxsw(void *, void *);

 
 
/* in file freebuf.c */
//extern	syscall	freebuf(char *);

/* in file freemem.c */
extern	syscall	freemem(char *, uint32);

/* in file getbuf.c */
//extern	char	*getbuf(bpid32);

/* in file getc.c */
extern	syscall	getc(did32);

/* in file getitem.c */
extern	pid32	getfirst(qid16);
extern	pid32	getlast(qid16);
extern	pid32	getitem(pid32);

/* in file getmem.c */
extern	char	*getmem(uint32);

/* in file getpid.c */
extern	pid32	getpid(void);

/* in file getprio.c */
extern	syscall	getprio(pid32);

/* in file getstk.c */
extern	char	*getstk(uint32);

/* in file getticks.c */
extern	uint32	getticks(void);

 
 

/* in file halt.S */
extern	void	halt(void);
 



extern	syscall	readSector(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	sector,
	  uint32	count		/* Length of buffer		*/
	);

extern	syscall	writeSector(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	sector,
	  uint32	count		/* Length of buffer		*/
	);

extern	syscall	toggle(
	  did32				/* Descriptor for device	*/
	);
extern	syscall	set(
	  did32		,		/* Descriptor for device	*/
	  char				/* Character to send		*/
	);
/* in file init.c */
extern	syscall	init(did32);

/* in file insert.c */
extern	status	insert(pid32, qid16, int32);

/* in file insertd.c */
extern	status	insertd(pid32, qid16, int32);



/* in file ioerr.c */
extern	devcall	ioerr(void);

/* in file ionull.c */
extern	devcall	ionull(void);
 
/* in file kill.c */
extern	syscall	kill(pid32);

/* in file lexan.c */
extern	int32	lexan(char *, int32, char *, int32 *, int32 [], int32 []);

 
 

 
/* in file mkbufpool.c */
//extern	bpid32	mkbufpool(int32, int32);

/* in file mount.c */
extern	syscall	mount(char *, char *, did32);
extern	int32	namlen(char *, int32);

/* in file naminit.c */
extern	status	naminit(void);

/* in file nammap.c */
extern	devcall	nammap(char *, char[], did32);
extern	did32	namrepl(char *, char[]);
extern	status	namcpy(char *, char *, int32);

/* in file namopen.c */
extern	devcall	namopen(struct dentry *, char *, char *);

/* in file newqueue.c */
extern	qid16	newqueue(void);

/* in file open.c */
extern	syscall	open(did32, char *, char *);

/* in file panic.c */
extern	void	panic(char *);

/* in file pci.c */
//extern	int32	pci_init(void);

/* in file pdump.c */
//extern	void	pdump(struct netpacket *);
//extern	void	pdumph(struct netpacket *);

/* in file platinit.c */
extern	void	platinit(void);


/* in file putc.c */
extern	syscall	putc(did32, char);

/* in file queue.c */
extern	pid32	enqueue(pid32, qid16);
extern	pid32	dequeue(qid16);


/* in file read.c */
extern	syscall	read(did32, char *, uint32);

/* in file ready.c */
extern	status	ready(pid32);

/* in file receive.c */
extern	umsg32	receive(void);

/* in file recvclr.c */
extern	umsg32	recvclr(void);

/* in file recvtime.c */
extern	umsg32	recvtime(int32);

/* in file resched.c */
extern	void	resched(void);
extern	status	resched_cntl(int32);

/* in file intr.S */
extern	void	restore(intmask);

/* in file resume.c */
extern	pri16	resume(pid32);

 
/* in file seek.c */
extern	syscall	seek(did32, uint32);

/* in file semcount.c */
extern	syscall	semcount(sid32);

/* in file semcreate.c */
extern	sid32	semcreate(int32);

/* in file semdelete.c */
extern	syscall	semdelete(sid32);

/* in file semreset.c */
extern	syscall	semreset(sid32, int32);

/* in file send.c */
extern	syscall	send(pid32, umsg32);

/* in file shell.c */
extern 	process shell(int nargs, char *arg[]);

/* in file signal.c */
extern	syscall	signal(sid32);

/* in file signaln.c */
extern	syscall	signaln(sid32, int32);

/* in file sleep.c */
extern	syscall	sleepms(int32);
extern	syscall	sleep(int32);

 
 

/* in file suspend.c */
extern	syscall	suspend(pid32);

 

/* in file unsleep.c */
extern	syscall	unsleep(pid32);

/* in file userret.c */
extern	void	userret(void);

/* in file wait.c */
extern	syscall	wait(sid32);

/* in file wakeup.c */
extern	void	wakeup(void);

/* in file write.c */
extern	syscall	write(did32, char *, uint32);

/* in file xdone.c */
extern	void	xdone(void);

/* in file yield.c */
extern	syscall	yield(void);



extern	devcall	initLED( struct dentry *);
extern	devcall	setLED( struct dentry *, char );
extern	devcall	toggleLED( struct dentry *);


 
extern	devcall	ttycontrol( struct dentry *, int32, int32, int32);

/* in file ttygetc.c */
// extern	devcall	ttygetc(struct dentry *);
extern	devcall	ttygetc( struct dentry *);

/* in file ttyhandle_in.c */
extern	void	ttyhandle_in(struct ttycblk *, char c);

/* in file ttyhandle_out.c */
extern	void	ttyhandle_out(struct ttycblk *);

/* in file ttyhandler.c */
extern	void	ttyhandler(uint32, char c, int tipo);

/* in file ttyinit.c */
// extern	devcall	ttyinit(struct dentry *);
extern	devcall	ttyinit( struct dentry *);

/* in file ttykickout.c */
extern	void	ttykickout();

/* in file ttyputc.c */
// extern	devcall	ttyputc(struct dentry *, char);
extern	devcall	ttyputc( struct dentry *, char);

/* in file ttyread.c */
// extern	devcall	ttyread(struct dentry *, char *, int32);
extern	devcall	ttyread( struct dentry *, char *, int32);

/* in file ttywrite.c */
// extern	devcall	ttywrite(struct dentry *, char *, int32);
extern	devcall	ttywrite( struct dentry *, char *, int32);

extern	devcall	initFlash( struct dentry *devptr);
extern	devcall	flashRead( struct dentry *devptr,char	*buff,int32,int32 );
extern	devcall	flashWrite( struct dentry *devptr,char	*buff,int32,int32 );
extern	devcall	flashControl(
	   struct dentry	*devptr,	/* Entry in device switch table	*/
	  int32	 func,			/* Function to perform		*/
	  int32	 arg1,			/* Argument 1 for request	*/
	  int32	 arg2			/* Argument 2 for request	*/
	);


