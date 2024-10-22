/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <xinu.h>


extern	devcall	ioerr(void);
extern	devcall	ionull(void);

/* Device independent I/O switch */

struct	dentry	devtab[NDEVS] =
{
/**
 * Format of entries is:
 * dev-number, minor-number, dev-name,
 * init, open, close,
 * read, write, seek,
 * getc, putc, control,
 * dev-csr-address, intr-handler, irq
 */

/* CONSOLE is tty */
	{ 0, 0, "CONSOLE",
	  (void *)ttyinit, (void *)ionull, (void *)ionull,
	  (void *)ttyread, (void *)ttywrite, (void *)ioerr,
	  (void *)ttygetc, (void *)ttyputc, (void *)ttycontrol,
	  (void *)0x0, (void *)ttyhandler, 53, 
	  (void *)ionull, (void *)ionull
	},

/* NULLDEV is null */
	{ 1, 0, "NULLDEV",
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0, 
	  (void *)ionull, (void *)ionull
	},

/* NAMESPACE is nam */
	{ 2, 0, "NAMESPACE",
	  (void *)naminit, (void *)namopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0,
	  (void *)ionull, (void *)ionull
	},

	{ 3, 0, "LED",
	  (void *)initLED, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)0x0, (void *)ioerr, 0 ,
	  (void *)setLED, (void *)toggleLED
	},

	{ 4, 0, "W25Q128",
	  (void *)initFlash, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)flashControl,
	  (void *)0x0, (void *)ioerr, 0 ,
	  (void *)ionull, (void *)ionull,
	  (void *)flashRead, (void *)flashWrite
	},
/* SPI1 is spi */
	/*{ 3, 0, "SPI1",
	  (void *)spiinit, (void *)ioerr, (void *)ioerr,
	  (void *)spiread, (void *)spiwrite, (void *)ioerr,
	  (void *)spigetc, (void *)spiputc, (void *)ioerr,
	  (void *)0x40013000, (void *)ioerr, 0 }*/
};
