#include <fat_filelib.h>
#include <stdint.h>


#define DRAM_SIZE 125000 // Tamaño de la memoria RAM simulada
#define BLOCK_SIZE 64
typedef struct {
    FILE *file;
    uint32_t size;
    char *p;
} Memory;


extern Memory mem;

uint32_t get_file_size(FILE *file);
Memory create_memory(const char *filename);
void free_memory(Memory *mem); 


int memory_read(uint32_t addr, void *buf, int len);
int memory_write(uint32_t addr, void *buf, int len);

void cache_write(uint32_t ofs, void *buf, uint32_t size);
void cache_read(uint32_t ofs, void *buf, uint32_t size);
void cache_get_stat(uint64_t *phit, uint64_t *paccessed);

