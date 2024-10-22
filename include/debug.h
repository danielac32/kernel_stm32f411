/* debug.h */

#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02

void hexdump(void *buffer, uint32 length, bool8 text);

void    hexdump2(
     void   *buffer,        /* Addresss of memory area  */
     uint32 length,         /* Length in bytes      */
     uint32 offset,
     bool8  canon           /* Print in ASCII or hex    */
    );
void debugbreak(void);
void debugret(void);
