/* ttycontrol.c - ttycontrol */

#include <xinu.h>
#include <w25qxxx.h>
/*------------------------------------------------------------------------
 *  ttycontrol  -  Control a tty device by setting modes
 *------------------------------------------------------------------------
 */
devcall	flashControl(
	   struct dentry	*devptr,	/* Entry in device switch table	*/
	  int32	 func,			/* Function to perform		*/
	  int32	 arg1,			/* Argument 1 for request	*/
	  int32	 arg2			/* Argument 2 for request	*/
	)
{

	 switch(func){
	 	case 0:
	 	{
	 		flash_info_t *flash_info;
	        flash_info = w25qxxx_drv.getcardinfo();
	        return flash_info->card_size;
	 	}
	 	default:
	 		return OK;
	 }
}
