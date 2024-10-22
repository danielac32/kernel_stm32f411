#ifndef STDDEF_H_INCLUDED
#define STDDEF_H_INCLUDED

typedef __PTRDIFF_TYPE__    ptrdiff_t;
typedef __SIZE_TYPE__       size_t;

#define NULL            (void *) 0x00000000
#define offsetof(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

#endif
