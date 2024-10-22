/* stdio.h - definintions and constants for standard I/O functions */
#include "kernel.h"
#include <stdarg.h>

#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02

/* Definintion of standard input/ouput/error used with shell commands */
 


/* Prototypes for formatted output functions */

extern	int32	fprintf(int, char *, ...);
extern	int32	printf(const char *, ...);
extern	int32	sprintf(char *, char *, ...);
extern void print(char *);
#define gets(buffer) gets_base(kgetch, buffer)

extern int lib_putc(char ch);
extern void lib_puts(char *s);
extern int lib_printf(const char *s, ...);
extern int lib_vprintf(const char *s, va_list vl);
extern int lib_vsnprintf(char *out, size_t n, const char *s, va_list vl);

extern void    panic (
     char   *msg            /* Message to display       */
    );
void hexDump(uint32 offset,char *desc, void *addr, int len);
void hexDump2(uint32 offset, void *addr, int len);