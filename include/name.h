/* name.h */

/* Constants that define the namespace mapping table sizes */
#if 0
#define	NM_PRELEN	64		/* Max size of a prefix string	*/
#define	NM_REPLLEN	96		/* Maximum size of a replacement*/
#define	NM_MAXLEN	256		/* Maximum size of a file name	*/
#define	NNAMES		40		/* Number of prefix definitions	*/
#endif

#define	NM_PRELEN	16		/* Max size of a prefix string	*/
// RAFA #define	NM_REPLLEN	8		/* Maximum size of a replacement*/
// RAFA #define	NM_MAXLEN	8		/* Maximum size of a file name	*/
// #define	NM_REPLLEN	16		/* Maximum size of a replacement*/
#define	NM_REPLLEN	1		/* Maximum size of a replacement*/
#define	NM_MAXLEN	16		/* Maximum size of a file name	*/
#define	NNAMES		5		/* Number of prefix definitions	*/

/* Definition of the name prefix table that defines all name mappings */

struct	nmentry	{			/* Definition of prefix table	*/
	char	nprefix[NM_PRELEN];	/* Null-terminated prefix	*/
	char	nreplace[NM_REPLLEN];	/* Null-terminated replacement	*/
	did32	ndevice;		/* Device descriptor for prefix	*/
};

extern	struct	nmentry	nametab[];	/* Table of name mappings	*/
extern	int32	nnames;			/* Number of entries allocated	*/
