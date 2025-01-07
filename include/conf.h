/* conf.h (GENERATED FILE; DO NOT EDIT) */

/* Device switch table declarations */

/* Device table entry */
struct	dentry	{
	int32   dvnum;
	int32   dvminor;
	char    *dvname;
	devcall (*dvinit) (struct dentry *);
	devcall (*dvopen) (struct dentry *, char *, char *);
	devcall (*dvclose)(struct dentry *);
	devcall (*dvread) (struct dentry *, void *, uint32);
	devcall (*dvwrite)(struct dentry *, void *, uint32);
	devcall (*dvseek) (struct dentry *, int32);
	devcall (*dvgetc) (struct dentry *);
	devcall (*dvputc) (struct dentry *, char);
	devcall (*dvcntl) (struct dentry *, int32, int32, int32);
	void    *dvcsr;
	void    (*dvintr)(void);
	byte    dvirq;
		//funciones solo para led
	int32 (*dvset) (struct dentry *,bool);
	int32 (*dvtoggle) (struct dentry *);
	//funciones para escribir en la flash
	devcall (*dvreadsector) (struct dentry *, void *, uint32 , uint32);
	devcall (*dvwritesector) (struct dentry *, void *, uint32 , uint32);
};

extern	struct	dentry	devtab[]; /* one entry per device */

/* Device name definitions */

#define CONSOLE              0	/* type tty      */
#define NULLDEV              1	/* type null     */
#define NAMESPACE            2	/* type nam      */
#define LED                  3	/* type nam      */
#define FLASHSPI             4	/* type nam      */
/* Control block sizes */

#define	Nnull	1
#define	Ntty	1
#define	Nnam	1
#define	Nspi	1

#define NDEVS 5


/* Configuration and Size Constants */

#define	NPROC	     12		/* number of user processes		*/
#define	NSEM	     12		/* number of semaphores			*/
