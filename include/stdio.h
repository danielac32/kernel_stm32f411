/* stdio.h - definintions and constants for standard I/O functions */


/* Prototypes for formatted input functions */

//extern	int	_doscan(char *,int *, int32 (*)(void),
//			int32 (*)(char), int, int);
//extern	int	sscanf(char *, char *, int);
//extern  int   fscanf(int, char *, int);
//#define	scanf(fmt, args)	fscanf(CONSOLE, fmt, args)

#include <stdbool.h>
/* Definintion of standard input/ouput/error used with shell commands */

#define	stdin	((proctab[currpid]).prdesc[0])
#define	stdout	((proctab[currpid]).prdesc[1])
#define	stderr	((proctab[currpid]).prdesc[2])


/* Prototypes for formatted output functions */

extern	int	fprintf(int, char *, ...);
extern	int	printf(const char *, ...);
extern	int	printf2(const char *, ...);
extern	int	sprintf(char *, char *, ...);


/* Prototypes for character input and output functions */

extern	int	fgetc(int);
extern	char	*fgets(char *, int, int);
extern	int	fputc(int, int);
extern	int	fputs(char *, int);
extern	int	putchar(int);
extern	int	getchar(void);

extern void    hexdump(
     void   *buffer,        /* Addresss of memory area  */
     uint32 length,         /* Length in bytes      */
     bool  canon           /* Print in ASCII or hex    */
    );
 

extern void    hexdump2(
     void   *buffer,        /* Addresss of memory area  */
     uint32 length,         /* Length in bytes      */
     uint32 offset,
     bool  canon           /* Print in ASCII or hex    */
    );

extern void hexDump2(uint32 offset, void *addr, int len) ;