/**
 * @file stdlib.h
 *
 * $Id: stdlib.h 2051 2009-08-27 20:55:09Z akoehler $
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */


#include <kernel.h>

#define RAND_MAX 2147483646

int abs(int);
long labs(long);
int atoi(char *);
long atol(char *);
void bzero(void *, int);
void qsort(char *, unsigned int, int, int (*)(void));
int rand(void);
void srand(unsigned int);
void *malloc(uint32);
void *
realloc(void *ptr, size_t len);
void free(void *);
float atof(const char* string);
char *itoa (int n);
void *aligned_malloc(size_t alignment,size_t bytes);
void aligned_free(void *p );