#include <xinu.h>
#include <w25qxxx.h>

devcall	flashWrite( struct dentry *devptr,char	*buff,int32 sector ,int32	count ){
	return w25qxxx_drv.write (buff,sector,count);
}