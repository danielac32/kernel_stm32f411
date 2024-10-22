/**
 * @file stdlib.h
 *
 * $Id: stdlib.h 2051 2009-08-27 20:55:09Z akoehler $
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <stddef.h>
#include <stdint.h>

int abs(int);
long labs(long);
int atoi(char *);
long atol(char *);
void bzero(void *, int);
void qsort(char *, unsigned int, int, int (*)(void));
int rand(void);
void srand(unsigned int);

void* realloc(void* ptr, size_t size);
void  *calloc(size_t nmemb, size_t size);
void *malloc(size_t );
void free(void *);
void abort(void);
int atoi(char buf[]);
char *itoa(int number, char *buf, uint32_t len, uint32_t base);
char *uitoa(uint32_t number, char *buf, uint32_t len, uint32_t base);
void ftoa(float num, float tolerance, char buf[], uint32_t n);
