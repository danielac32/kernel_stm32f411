/* debug.c  -  hexdump_print, hexdump */

#include <xinu.h>
//#include <stdio.h>
//#include <debug.h>

#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02


static void hexdump_print(byte, byte);
static void hexdump_print(
	 byte	data,			/* Item to print		*/
	 byte	mode			/* ASCII or hex mode		*/
	)
{
    switch (mode)
    {
    case DEBUG_ASCII:
        data = (' ' <= data && data <= '~') ? data : '.';
        printf( "%c", data);
        break;
    case DEBUG_HEX:
        printf( "%02x ", data);
        break;
    default:
        break;
    }
}


void	hexdump(
	 void	*buffer,		/* Addresss of memory area	*/
	 uint32	length,			/* Length in bytes		*/
	 bool	canon			/* Print in ASCII or hex	*/
	)
{
    uint32 m, n, remain;

    byte *b = (byte *)buffer;

    for (n = 0; n < length; n += 0x10) {
        printf( "%08x ",  n);

        remain = length - n;

        for (m = 0; m < remain && m < 0x10; m++) {
            if (m % 0x08 == 0) {
                printf( " ");
            }
            hexdump_print(b[n + m], DEBUG_HEX);
        }

        /* Pad the rest if needed */
        if (remain < 0x10) {
            for (m = 0; m < 0x10 - remain; m++) {
                if ((0 != m) && (0 == m % 0x08)) {
                    printf( " ");
                }
                printf( "   ");
            }
        }

        if (canon == TRUE) {
            printf( " |");
            for (m = 0; m < remain && m < 0x10; m++) {
                hexdump_print(b[n + m], DEBUG_ASCII);
            }
            printf( "|");
        }
        printf( "\n");
    }
}



void    hexdump2(
     void   *buffer,        /* Addresss of memory area  */
     uint32 length,         /* Length in bytes      */
     uint32 offset,
     bool  canon           /* Print in ASCII or hex    */
    )
{
    uint32 m, n, remain;

    byte *b = (byte *)buffer;

    for (n = 0; n < length; n += 0x10) {
        printf( "%08x ",  n+offset);

        remain = length - n;

        for (m = 0; m < remain && m < 0x10; m++) {
            if (m % 0x08 == 0) {
                printf( " ");
            }
            hexdump_print(b[n + m], DEBUG_HEX);
        }

        /* Pad the rest if needed */
        if (remain < 0x10) {
            for (m = 0; m < 0x10 - remain; m++) {
                if ((0 != m) && (0 == m % 0x08)) {
                    printf( " ");
                }
                printf( "   ");
            }
        }

        if (canon == TRUE) {
            printf( " |");
            for (m = 0; m < remain && m < 0x10; m++) {
                hexdump_print(b[n + m], DEBUG_ASCII);
            }
            printf( "|");
        }
        printf( "\n");
    }
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
            if (i != 0){
                printf("  %s\n", buff);
                
            }

            // Output the offset.
            printf("  %04x ", offset+i);
            
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);
        

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