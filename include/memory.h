/* memory.h - roundmb, truncmb, freestk */
#include <kernel.h>
#define MAXADDR		0x2001f400		/* 64kB SRAM */
#define HANDLERSTACK	1024			/* Size reserved for stack in Handler mode */
#define	PAGE_SIZE	1024 // TODO: unused?

/*----------------------------------------------------------------------
 * roundmb, truncmb - Round or truncate address to memory block size
 *----------------------------------------------------------------------
 */
#define	roundmb(x)	(char *)( (7 + (uint32)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  Free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define	freestk(p,len)	freemem((char *)((uint32)(p)		\
				- ((uint32)roundmb(len))	\
				+ (uint32)sizeof(uint32)),	\
				(uint32)roundmb(len) )

struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	uint32	mlength;		/* Size of blk (includes memblk)*/
	};
extern	struct	memblk	memlist;	/* Head of free memory list	*/
extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/


/* Added by linker */

extern	unsigned int	_text;			/* Start of text segment	*/
extern	unsigned int	_etext;			/* End of text segment		*/
extern	unsigned int	_sidata;
extern	unsigned int	_sdata;			/* Start of data segment	*/
extern	unsigned int	_edata;			/* End of data segment		*/
extern	unsigned int	_sbss;			/* Start of bss segment		*/
extern	unsigned int	_ebss;			/* End of bss segment		*/
extern	unsigned int	end;			/* End of program		*/
uint32 heap_free(void);