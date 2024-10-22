/* STM32 Timer
General purpose timer */

extern	uint32	clktime;	/* current time in secs since boot	*/
extern  uint32  count1000;      /* ms since last clock tick             */

extern	qid16	sleepq;		/* queue for sleeping processes		*/
extern	int32	slnonempty;	/* nonzero if sleepq is nonempty	*/
extern	int32	*sltop;		/* ptr to key in first item on sleepq	*/
extern	uint32	preempt;	/* preemption counter			*/
