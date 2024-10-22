/**
 * @file malloc.c
 * @provides malloc.c.
 * This file is deprecated in favor of mem/malloc.c and the user heap
 * allocator.  However, it remains here for backup purposes.
 *
 * $Id: malloc.c 2065 2009-09-04 21:44:36Z brylow $
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>


/**
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 * @param nbytes number of bytes requested
 * @return pointer to region on success, NULL on failure
 */
void *malloc(uint32 nbytes)
{
    struct memblk *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == nbytes)
    {
        return NULL;
    }

    /* make room for accounting info */
    nbytes += sizeof(struct memblk);

    /* acquire memory from kernel */
    pmem = (struct memblk *)getmem(nbytes);
    if (SYSERR == (uint32)pmem)
    {
        return NULL;
    }

    /* set accounting info */
    pmem->mnext = pmem;
    pmem->mlength = nbytes;

    return (void *)(pmem + 1);  /* +1 to skip accounting info */
}

void free(void *pmem)
{
    struct memblk *block;

    /* block points at the memblock we want to free */
    block = (struct memblk *)pmem;

    /* back up to accounting information */
    block--;

    /* don't memfree if we fail basic checks */
    if (block->mnext != block)
    {
        return;
    }

    freemem((char *)block, block->mlength);
}
