/* memory.h - roundmb, truncmb, freestk */
 
#include "queue.h"


extern	unsigned int	_text_start;			/* Start of text segment	*/
extern	unsigned int	_text_end;			/* End of text segment		*/
extern	unsigned int	_data_start;			/* Start of data segment	*/
extern	unsigned int	_data_end;			/* End of data segment		*/
extern	unsigned int	_bss_start;			/* Start of bss segment		*/
extern	unsigned int	_bss_end;			/* End of bss segment		*/
extern	unsigned int	end;			/* End of program		*/
extern unsigned int max;


//extern unsigned char fs;
//extern unsigned int _heap_start;
//extern unsigned int _heap_size;

#define MAXADDR 0x8007d000//0x80000000 + 512000//1000000
#define KMALLOC_START  (void*)&end
#define KMALLOC_LENGTH (MAXADDR) - (unsigned int)(&end)

void malloc_init(void *start, int length);
void malloc_debug();


#define KUNIT sizeof(struct kmalloc_chunk)

#define KMALLOC_STATE_FREE 0xa1a1a1a1
#define KMALLOC_STATE_USED 0xbfbfbfbf

struct kmalloc_chunk {
	int state;
	int length;
	//char pid;
	struct kmalloc_chunk *next;
	struct kmalloc_chunk *prev;
};

extern struct kmalloc_chunk *head;

typedef struct chunk_s {
    struct chunk_s  *next;
    size_t          size;
    char            free;
    //char 			pid;
} chunk_t;


extern chunk_t *it;

extern sem_t sem_malloc;

/*
void* do_malloc (chunk_t* it, size_t len);
void do_free (chunk_t* it, void *p);*/
void chunklist_init (uint32 heap_size);
void info();