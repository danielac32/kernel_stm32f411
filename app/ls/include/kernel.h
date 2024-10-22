/* kernel.h */


typedef int si_int;
typedef unsigned su_int;

typedef long long di_int;
typedef unsigned long long du_int;

typedef unsigned int	UINT;	/* int must be 16-bit or 32-bit */
typedef unsigned char	BYTE;	/* char must be 8-bit */
typedef unsigned short	WORD;	/* 16-bit unsigned integer */
typedef unsigned short	WCHAR;	/* 16-bit unsigned integer */
typedef unsigned long	DWORD;	/* 32-bit unsigned integer */
typedef unsigned long 	dword;
typedef unsigned short	word;	/* 16-bit unsigned integer */
typedef unsigned int 	size_t;
typedef unsigned int 	uint;
typedef unsigned short	ushort;
typedef long int off_t;

typedef unsigned char          uint8_t;
typedef unsigned short int     uint16_t;
typedef unsigned int           uint32_t;
typedef unsigned long long int uint64_t;
typedef unsigned long uintptr_t;

typedef  int           int32_t;
typedef  char          int8_t;
typedef  short int     int16_t;


typedef	unsigned char	byte;
typedef	unsigned char	uint8;
typedef	int		int32;
typedef	short		int16;
typedef	unsigned int	uint32;
typedef	unsigned short	uint16;
typedef	unsigned long long uint64;
typedef int tid_typ; 
#define REGBYTES 4

typedef	byte	bool8;		/* Boolean type				*/

 
//typedef unsigned long uintptr_t;

/* Function declaration return types */

#define local	static		/* Local procedure or variable declar.	*/

/* Boolean constants */
#define bool   _Bool
#define true   1
#define false  0

#define FALSE	0		/* Boolean False			*/
#define TRUE	1		/* Boolean True				*/

/* Null pointer, character, and string definintions */

#define NULL	(void *)0		/* null pointer for linked lists	*/
#define NULLCH	'\0'		/* null character			*/
#define	NULLSTR	""		/* null string				*/

/* Universal return constants */

#define OK	( 1)		/* normal system call return		*/
#define	SYSERR	(-1)		/* system call failed			*/
#define	EOF	(-2)		/* End-of-file (usually from read)	*/
#define	TIMEOUT	(-3)		/* system call timed out		*/

 
#define	MINSTK	400		/* minimum stack size in bytes		*/

#define	CONTEXT	64		/* bytes in a function call context on	*/
				/* the run-time stack			*/
#define	QUANTUM	10		/* time slice in milliseconds		*/

/* Size of the stack for the null process */

#define	NULLSTK		4096	/* stack size for null process		*/

/* Prototypes of I/O functions used throughout the kernel */

uint32	kprintf(const char *fmt, ...);
uint32	kputc(byte);
uint32	kgetc(void);








