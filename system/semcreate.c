/* semcreate.c - semcreate, newsem */

#include <xinu.h>

local	sid32	newsem(void);

/*------------------------------------------------------------------------
 *  semcreate  -  Create a new semaphore and return the ID to the caller
 *------------------------------------------------------------------------
 */
sid32	semcreate(
	  int32		count		/* Initial semaphore count	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	sid32	sem;			/* Semaphore ID to return	*/

	mask = disable();

	if (count < 0 || ((sem=newsem())==SYSERR)) {
		restore(mask);
		return SYSERR;
	}
	semtab[sem].scount = count;	/* Initialize table entry	*/

	restore(mask);
	return sem;
}

/*------------------------------------------------------------------------
 *  newsem  -  Allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
local	sid32	newsem(void)
{
	static	sid32	nextsem = 0;	/* Next semaphore index to try	*/
	sid32	sem;			/* Semaphore ID to return	*/
	int32	i;			/* Iterate through # entries	*/

	for (i=0 ; i<NSEM ; i++) {
		sem = nextsem++;
		if (nextsem >= NSEM)
			nextsem = 0;
		if (semtab[sem].sstate == S_FREE) {
			semtab[sem].sstate = S_USED;
			return sem;
		}
	}
	return SYSERR;
}

/*------------------------------------------------------------------------
 *  semcount  -  Return the count of a semaphore (because any integer is
 *		   possible, return of SYSERR may be ambiguous)
 *------------------------------------------------------------------------
 */
syscall semcount(
	  sid32		semid		/* ID of semaphore to use	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	int32	count;			/* Current sempahore count	*/

	mask = disable();
	
	if (isbadsem(semid) || semtab[semid].sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}
	count = semtab[semid].scount;
	restore(mask);
	return count;
}

/*------------------------------------------------------------------------
 * semdelete  -  Delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
syscall	semdelete(
	  sid32		sem		/* ID of semaphore to delete	*/
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	sentry *semptr;		/* Ptr to semaphore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}
	
	semptr = &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}
	semptr->sstate = S_FREE;

	resched_cntl(DEFER_START);
	while (semptr->scount++ < 0) {	/* Free all waiting processes	*/
		ready(getfirst(semptr->squeue));
	}
	resched_cntl(DEFER_STOP);
	restore(mask);
	return OK;
}

/*------------------------------------------------------------------------
 *  semreset  -  Reset a semaphore's count and release waiting processes
 *------------------------------------------------------------------------
 */
syscall	semreset(
	  sid32		sem,		/* ID of semaphore to reset	*/
	  int32		count		/* New count (must be >= 0)	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	sentry *semptr;		/* Ptr to semaphore table entry */
	qid16	semqueue;		/* Semaphore's process queue ID	*/
	pid32	pid;			/* ID of a waiting process	*/

	mask = disable();

	if (count < 0 || isbadsem(sem) || semtab[sem].sstate==S_FREE) {
		restore(mask);
		return SYSERR;
	}
	
	semptr = &semtab[sem];
	semqueue = semptr->squeue;	/* Free any waiting processes */
	resched_cntl(DEFER_START);
	while ((pid=getfirst(semqueue)) != EMPTY)
		ready(pid);
	semptr->scount = count;		/* Reset count as specified */
	resched_cntl(DEFER_STOP);
	restore(mask);
	return OK;
}


