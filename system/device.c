#include <xinu.h>


syscall	readSector(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	sector,
	  uint32	count		/* Length of buffer		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/
	
	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvreadsector) (devptr, buffer, sector,count);
	restore(mask);
	return retval;
}
 
syscall	writeSector(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	sector,
	  uint32	count		/* Length of buffer		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvwritesector) (devptr, buffer, sector,count);
	restore(mask);
	return retval;
}

syscall	set(
	  did32		descrp,		/* Descriptor for device	*/
	  char		ch		/* Character to send		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvset) (devptr, ch);
	restore(mask);
	return retval;
}

syscall	toggle(
	  did32		descrp		/* Descriptor for device	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvtoggle) (devptr);
	restore(mask);
	return retval;
}

/*------------------------------------------------------------------------
 *  init  -  Initialize a device and its driver
 *------------------------------------------------------------------------
 */
syscall	init(
	  did32		descrp		/* Descriptor for device	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvinit) (devptr);
	restore(mask);
	return retval;
}

/*------------------------------------------------------------------------
 *  close  -  Close a device
 *------------------------------------------------------------------------
 */
syscall	close(
	  did32		descrp		/* Descriptor for device	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct	dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvclose) (devptr);
	restore(mask);
	return retval;
}

/*------------------------------------------------------------------------
 *  getc  -  Obtain one byte from a device
 *------------------------------------------------------------------------
 */
syscall	getc(
	  did32		descrp		/* Descriptor for device	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvgetc) (devptr);
	restore(mask);
	return retval;
}

/*------------------------------------------------------------------------
 *  ioerr  -  Return an error status (used for "error" entries in devtab)
 *------------------------------------------------------------------------
 */
devcall	ioerr(void)
{
	return SYSERR;
}

/*------------------------------------------------------------------------
 *  ionull  -  Do nothing (used for "don't care" entries in devtab)
 *------------------------------------------------------------------------
 */
devcall	ionull(void)
{
	return OK;
}
/*------------------------------------------------------------------------
 *  open  -  Open a device (some devices ignore name and mode parameters)
 *------------------------------------------------------------------------
 */
syscall	open(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*name,		/* Name to use, if any		*/
	  char		*mode		/* Mode for device, if any	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvopen) (devptr, name, mode);
	restore(mask);
	return retval;
}
/*------------------------------------------------------------------------
 *  putc  -  Send one character of data (byte) to a device
 *------------------------------------------------------------------------
 */
syscall	putc(
	  did32		descrp,		/* Descriptor for device	*/
	  char		ch		/* Character to send		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvputc) (devptr, ch);
	restore(mask);
	return retval;
}
/*------------------------------------------------------------------------
 *  read  -  Read one or more bytes from a device
 *------------------------------------------------------------------------
 */
syscall	read(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	count		/* Length of buffer		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/
	
	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvread) (devptr, buffer, count);
	restore(mask);
	return retval;
}
/*------------------------------------------------------------------------
 *  write  -  Write one or more bytes to a device
 *------------------------------------------------------------------------
 */
syscall	write(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	count		/* Length of buffer		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvwrite) (devptr, buffer, count);
	restore(mask);
	return retval;
}

/*------------------------------------------------------------------------
 *  seek  -  Position a random access device
 *------------------------------------------------------------------------
 */
syscall	seek(
	  did32		descrp,		/* Descriptor for device	*/
	  uint32	pos		/* Position			*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvseek) (devptr, pos);
	restore(mask);
	return retval;
}

/*------------------------------------------------------------------------
 *  control  -  Control a device or a driver (e.g., set the driver mode)
 *------------------------------------------------------------------------
 */
syscall	control(
	  did32		descrp,		/* Descriptor for device	*/
	  int32		func,		/* Specific control function	*/
	  int32		arg1,		/* Specific argument for func	*/
	  int32		arg2		/* Specific argument for func	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	int32		retval;		/* Value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvcntl) (devptr, func, arg1, arg2);
	restore(mask);
	return retval;
}

did32	getdev(
	  char		*devname	/* Name of the device		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	did32		id;		/* Value to return to caller	*/

	mask = disable();
	for (id = 0; id < NDEVS; id++) {
		if (strncmp(devname, devtab[id].dvname, DEVNAMLEN)) {
			restore(mask);
			return id;
		}
	}
	restore(mask);
	return (did32) SYSERR;
}
