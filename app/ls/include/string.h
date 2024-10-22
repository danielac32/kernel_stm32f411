/* string.h */


extern char * strtok(char *, const char *);
extern	char	*strncpy(char *, const char *, int);
extern	char	*strncat(char *, const char *, int);
extern	int	strncmp(const char *, const char *, int);
extern	char	*strchr(const char *, int);
extern	char	*strrchr(const char *, int);
extern	char	*strstr(const char *, const char *);
extern	int	strnlen(const char *, int);
extern	int	strlen(const char *str);
extern int	strcmp(const char	*,const char *);
extern int	strcpy(char *,const char *);
extern char * strcat(char *, const char *);
//extern void *memmove (void *, const void *, int);
extern void	*memset(void*,int,int);
//extern int tolower(int c);
//extern int toupper(int c);
extern void strrev(unsigned char *);

/* in file memcpy.c */
extern	void	*memcpy(void *, const void *, int);

/* in file memcpy.c */
extern int memcmp(const void *s1, const void *s2, int n);


/* in file memset.c */
extern  void    *memset(void *, int, int);
extern int strcspn(const char *, register const char *);
extern int
strspn (const char *s1,
    const char *s2);
extern char *strdup (const char *s);