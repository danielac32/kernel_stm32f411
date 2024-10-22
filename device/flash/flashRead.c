#include <xinu.h>
#include <w25qxxx.h>

devcall	flashRead( struct dentry *devptr,char	*buff,int32 sector ,int32	count ){
	return w25qxxx_drv.read (buff,sector,count);
}