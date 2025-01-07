

#include <xinu.h>
#include <mem.h>
#include <fat_filelib.h>
 


extern void exit();

uint32_t get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END); // Moverse al final del archivo
    uint32_t size = ftell(file); // Obtener la posición actual
    fseek(file, 0, SEEK_SET);//rewind(file); // Volver al inicio del archivo
    return size; // Retornar el tamaño
}

#if 1

Memory create_memory(const char *filename) {
    Memory mem;

    char *tmp=full_path(filename);

    mem.file = fopen(tmp, "rb"); // Abrir el archivo en modo lectura
    if (mem.file == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        exit();
    }

    // Obtener el tamaño del archivo original
    mem.size = get_file_size(mem.file);
    mem.p = (char *)malloc(90000);
    memset(mem.p,0,90000);
    uint8_t buffer[BLOCK_SIZE]; // Buffer para copiar datos
    size_t bytes_read;
    int part=0;
    uint32 br=0;
    fread(mem.p, mem.size,1,mem.file);

   // while(part<mem.size){
     //       br=fread(buffer, BLOCK_SIZE,1,mem.file);
            //fwrite(buffer, BLOCK_SIZE,1,dram_file);
       //     memcpy(mem.p,buffer,br);
            //memset(buffer,0,br);
         //   part += br;
          
    //}
 
    fclose(mem.file);
    //hexDump2(0,mem.p,mem.size);
    printf("END\n");
    return mem;
}

#else

Memory create_memory(const char *filename) {
    Memory mem;


    char *tmp=full_path(filename);
    

    printf("mount vram..\n");

    mem.file = fopen(tmp, "rb"); // Abrir el archivo en modo lectura
    if (mem.file == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        exit();
    }

    // Obtener el tamaño del archivo original
    mem.size = get_file_size(mem.file);

    // Crear o sobrescribir el archivo "dram"
    const char *dram_filename = full_path("dram");
    remove(dram_filename); // Eliminar "dram" si existe
    FILE *dram_file = fopen(dram_filename, "wb");
    if (dram_file == NULL) {
        printf("Error: No se pudo crear el archivo 'dram'.\n");
        fclose(mem.file);
        exit();
    }
    
    uint8_t buffer[BLOCK_SIZE]; // Buffer para copiar datos
    size_t bytes_read;
    int part=0;
    uint32_t target_size = DRAM_SIZE;
    while(part<target_size){
         fwrite(buffer, BLOCK_SIZE,1,dram_file);
         part += BLOCK_SIZE;
    }
    part=0;
    fseek(dram_file, 0, SEEK_SET);
    while(part<mem.size){
            fread(buffer, BLOCK_SIZE,1,mem.file);
            fwrite(buffer, BLOCK_SIZE,1,dram_file);
            memset(buffer,0,BLOCK_SIZE);
            part += BLOCK_SIZE;
    }


    /*const uint32_t target_size = 125000;
    if (mem.size < target_size) {
        uint8_t *zero_data = (uint8_t *)calloc(target_size - mem.size, sizeof(uint8_t));
        fwrite(zero_data, sizeof(uint8_t), target_size - mem.size, dram_file);
        free(zero_data);
    }*/

    //while ((bytes_read = fread(buffer, sizeof(uint8_t), BLOCK_SIZE, mem.file)) > 0) {
      //  fwrite(buffer, sizeof(uint8_t), bytes_read, dram_file);
    //}
    fclose(mem.file);
    fclose(dram_file); // Cerrar el archivo "dram"
    //rewind(mem.file);  // Volver al inicio del archivo original
    printf("END\n");
    return mem;
}


 
int memory_read(uint32_t addr, void *buf, int len){
    FILE *dram_file = fopen(full_path("dram"), "r+b");
    fseek(dram_file, addr, SEEK_SET);
    fread(buf, len,1, dram_file);
    fclose(dram_file);
    return 0;
}
int memory_write(uint32_t addr, void *buf, int len){
    FILE *dram_file = fopen(full_path("dram"), "w+b"); // Abrir en modo lectura y escritura
    if (dram_file == NULL) {
        printf("Error: No se pudo abrir el archivo 'dram'.\n");
        return -1;
    }
    fseek(dram_file, addr, SEEK_SET); // Mover el puntero al offset deseado
    fwrite(buf, len,1, dram_file);   
    fclose(dram_file);
    return 0;
}




#define CACHESIZE   4096
struct cacheline {
    uint8_t data[64];
};

static uint64_t accessed, hit;
static uint32_t tags[CACHESIZE/64/2][2];
static struct cacheline cachelines[CACHESIZE/64/2][2];

/*
 * bit[0]: valid
 * bit[1]: dirty
 * bit[2]: for LRU
 * bit[3:10]: reserved
 * bit[11:31]: tag
 */
#define VALID       (1 << 0)
#define DIRTY       (1 << 1)
#define LRU     (1 << 2)
#define LRU_SFT     2
#define TAG_MSK     0xfffff800

/*
 * bit[0: 5]: offset
 * bit[6: 10]: index
 * bit[11: 31]: tag
 */
static inline int get_index(uint32_t addr)
{
    return (addr >> 6) & 0x1f;
}

void cache_write(uint32_t ofs, void *buf, uint32_t size)
{
    if (((ofs | (64 - 1)) != ((ofs + size - 1) | (64 - 1))))
        printf("write cross boundary, ofs:%x size:%x\n", ofs, size);

    int ti, i, index = get_index(ofs);
    uint32_t *tp;
    uint8_t *p;

    ++accessed;

    for (i = 0; i < 2; i++) {
        tp = &tags[index][i];
        p = cachelines[index][i].data;
        if (*tp & VALID) {
            if ((*tp & TAG_MSK) == (ofs & TAG_MSK)) {
                ++hit;
                ti = i;
                break;
            } else {
                if (i != 1)
                    continue;

                ti = 1 - ((*tp & LRU) >> LRU_SFT);
                tp = &tags[index][ti];
                p = cachelines[index][ti].data;

                if (*tp & DIRTY) {
                    memory_write(*tp & ~0x3f, p, 64);
                }
                memory_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            memory_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(p + (ofs & 0x3f), buf, size);
    *tp |= DIRTY;
}

void cache_read(uint32_t ofs, void *buf, uint32_t size)
{
    if (((ofs | (64 - 1)) != ((ofs + size - 1) | (64 - 1))))
        printf("read cross boundary, ofs:%x size:%x\n", ofs, size);

    int ti, i, index = get_index(ofs);
    uint32_t *tp;
    uint8_t *p;

    ++accessed;

    for (i = 0; i < 2; i++) {
        tp = &tags[index][i];
        p = cachelines[index][i].data;
        if (*tp & VALID) {
            if ((*tp & TAG_MSK) == (ofs & TAG_MSK)) {
                ++hit;
                ti = i;
                break;
            } else {
                if (i != 1)
                    continue;

                ti = 1 - ((*tp & LRU) >> LRU_SFT);
                tp = &tags[index][ti];
                p = cachelines[index][ti].data;

                if (*tp & DIRTY) {
                    memory_write(*tp & ~0x3f, p, 64);
                }
                memory_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            memory_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(buf, p + (ofs & 0x3f), size);
}

void cache_get_stat(uint64_t *phit, uint64_t *paccessed)
{
    *phit = hit;
    *paccessed = accessed;
}
#endif