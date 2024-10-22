#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED


#include <stdint.h>
#include <stddef.h>

int chrnlst(char c, const char *l);
char * strtok(char *, const char *);
char	*strncpy(char *, const char *, int);
char	*strncat(char *, const char *, int);
int	strncmp(const char *, const char *, int);
char	*strchr(const char *, int);
char	*strrchr(const char *, int);
char	*strstr(const char *, const char *);
int	strnlen(const char *, int);
int	strlen(const char *str);
int	strcmp(const char	*,const char *);
int	strcpy(char *,const char *);
char * strcat(char *, const char *);
void memmove(void *, const void *, size_t );
void strrev(unsigned char *);
void strreverse(char *s);
/* in file memcpy.c */
void	*memcpy(void *, const void *, int);
void *memchr(const void *, int , size_t );
int memcmp(const void *, const void *, int );
/* in file memset.c */
//void    *memset(void *, int, int);
void memset(void *p, uint8_t value, uint32_t size);
int strcspn(const char *, register const char *);
int strspn (const char *,const char *);
char *strndup(const char *str, int n);
char *strdup(const char *str);

#endif