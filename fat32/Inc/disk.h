
/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2014          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

/*---------------------------------------*/
/* Prototypes for disk control functions */

int sd_init(void);
int sd_writesector(unsigned int sector, unsigned char *buffer, unsigned int sector_count);
int sd_readsector(unsigned int sector, unsigned char *buffer, unsigned int sector_count);


#endif

