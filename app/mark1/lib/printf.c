#include <stdarg.h>
#include <kernel.h>
#include <riscv.h>
#include <queue.h>
#include <process.h>
#include <syscall.h>
 

extern	void _doprnt(char *, va_list, int (*)(int));
extern void _fdoprnt(char *, va_list, int (*func) (int, int), int);

static int sprntf(int, int);


int lib_putc(int ch)
{
    //UART=ch;
    syscall(XINU_PUTC,ch);
    //sem_wait(&uart_tx);
   //csr_put(ch);
    //sem_signal(&uart_tx);
    return 0;
}

void print(char *s){
    //sem_wait(&uart_tx);
    while(*s){
        UART=*s++;
        //syscall(XINU_PUTC,*s++);
    }
}
uint32 kprintf(const char *fmt, ...)
{
    va_list ap;
 
    va_start(ap, fmt);
    _doprnt((char *)fmt, ap, lib_putc);
    va_end(ap);
  
    return OK;
}

static int  sprntf(
          int       acpp,
          int       ac
        )
{
    char **cpp = (char **)acpp;
    char c = (char)ac;

    return (*(*cpp)++ = c);
}


int32 sprintf(
      char      *str,       /* output string        */
      char      *fmt,       /* format string        */
      ...
    )
{
    va_list ap;
    char *s;

    s = str;
    va_start(ap, fmt);
    _fdoprnt(fmt, ap, sprntf, (int)&s);
    va_end(ap);
    *s++ = '\0';

    return ((int)str);
}



int32 printf(const char *fmt, ...){
   va_list ap; 
    
   va_start(ap, fmt);
   _doprnt((char *)fmt, ap, lib_putc);
   va_end(ap);
 
   return OK;
}

void hexDump2(uint32 offset, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

void hexDump(uint32 offset,char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Output description if given.
    if (desc != NULL)
        printf("%x (%s):\n", addr, desc);
    else
        printf("%x:\n", addr);
    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}





int lib_vsnprintf(char *out, size_t n, const char *s, va_list vl)
{
    int format = 0;
    int longarg = 0;
    size_t pos = 0;
    for (; *s; s++)
    {
        if (format)
        {
            switch (*s)
            {
            case 'l':
            {
                longarg = 1;
                break;
            }
            case 'p':
            {
                longarg = 1;
                if (out && pos < n)
                {
                    out[pos] = '0';
                }
                pos++;
                if (out && pos < n)
                {
                    out[pos] = 'x';
                }
                pos++;
            }
            case 'x':
            {
                long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                int hexdigits = 2 * (longarg ? sizeof(long) : sizeof(int)) - 1;
                for (int i = hexdigits; i >= 0; i--)
                {
                    int d = (num >> (4 * i)) & 0xF;
                    if (out && pos < n)
                    {
                        out[pos] = (d < 10 ? '0' + d : 'a' + d - 10);
                    }
                    pos++;
                }
                longarg = 0;
                format = 0;
                break;
            }
            case 'd':
            {
                long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                if (num < 0)
                {
                    num = -num;
                    if (out && pos < n)
                    {
                        out[pos] = '-';
                    }
                    pos++;
                }
                long digits = 1;
                for (long nn = num; nn /= 10; digits++)
                    ;
                for (int i = digits - 1; i >= 0; i--)
                {
                    if (out && pos + i < n)
                    {
                        out[pos + i] = '0' + (num % 10);
                    }
                    num /= 10;
                }
                pos += digits;
                longarg = 0;
                format = 0;
                break;
            }
            case 's':
            {
                const char *s2 = va_arg(vl, const char *);
                while (*s2)
                {
                    if (out && pos < n)
                    {
                        out[pos] = *s2;
                    }
                    pos++;
                    s2++;
                }
                longarg = 0;
                format = 0;
                break;
            }
            case 'c':
            {
                if (out && pos < n)
                {
                    out[pos] = (char)va_arg(vl, int);
                }
                pos++;
                longarg = 0;
                format = 0;
                break;
            }
            default:
                break;
            }
        }
        else if (*s == '%')
        {
            format = 1;
        }
        else
        {
            if (out && pos < n)
            {
                out[pos] = *s;
            }
            pos++;
        }
    }
    if (out && pos < n)
    {
        out[pos] = 0;
    }
    else if (out && n)
    {
        out[n - 1] = 0;
    }
    return pos;
}

static char out_buf[2000]; // buffer for lib_vprintf()

int lib_vprintf(const char *s, va_list vl)
{


 
    int res = lib_vsnprintf(NULL, -1, s, vl);
    if (res + 1 >= sizeof(out_buf))
    {
        print("error: lib_vprintf() output string size overflow\n");
        while (1)
        {
        }
    }
    lib_vsnprintf(out_buf, res + 1, s, vl);
    
    print(out_buf);
     
    return res;
}

int lib_printf(const char *s, ...)
{
    int res = 0;
    va_list vl;
    //sem_wait(&uart_tx);
    
    va_start(vl, s);
    res = lib_vprintf(s, vl);
    va_end(vl);
    //sem_signal(&uart_tx);

    return res;
}

void    panic (
     char   *msg            /* Message to display       */
    )
{
    //disable();          /* Disable interrupts       */
    kprintf("\n\n\rpanic: %s\n\n", msg);
    SYSCON = 0x5555; 
}