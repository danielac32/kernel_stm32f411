#include <interrupt.h>
#include <stdio.h>
#include <fat_filelib.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>



ELF32_shdr *elf_find_section(ELF32_hdr *ehdr, const char *name)
{
    ELF32_shdr *shdr = (ELF32_shdr *)((char *)ehdr + ehdr->e_shoff);
    ELF32_shdr *shdr_str = (ELF32_shdr *)((char *)ehdr + ehdr->e_shoff +
        ehdr->e_shstrndx * ehdr->e_shentsize);

    for (elf32_half i = 0; i < ehdr->e_shnum; i++) {
        char *section = ((char *)ehdr + shdr_str->sh_off) +
            shdr->sh_name;
        if (!strcmp(section, name))
            return shdr;

        shdr = (ELF32_shdr *)((char *)shdr + ehdr->e_shentsize);
    }

    return 0;
}



uint32 elf_execve(const char *file, exec_img *res)
{
    // Open the given file
    FILE* fd;
    uint32 q = disable();
    if (!(fd = fopen(file,"r"))){
        kprintf("not found %s\n",file);
        restore(q);
        return -1;
    }

    // Get file size
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    // Read entire file
    char *elfData = malloc(fileLength);
    if (fread((uint8 *)elfData, fileLength,1,fd) != fileLength) {
        fclose(fd);
        free(elfData);
        restore(q);
        return -2;
    }


    // ELF signature check
    if (elfData[0] != '\177' || elfData[1] != 'E' || elfData[2] != 'L' ||
        elfData[3] != 'F') {
        fclose(fd);
        free(elfData);
        restore(q);
        return -3;
    }


    uint32 ELF_OFFSET = 0;
    
    // Load program headers
    ELF32_hdr *ehdr = (ELF32_hdr *)elfData;
    Elf32_Phdr *phdr = (Elf32_Phdr *)(elfData + ehdr->e_phoff);

    // Search for the loadable program header
    for (elf32_half i = 0; i < ehdr->e_phnum; i++) {
        if (phdr->p_type == PT_LOAD) {
            // There should only be one PT_LOAD section...
            if (ELF_OFFSET != 0)
                while (1);
            ELF_OFFSET = (uint32)malloc(phdr->p_memsz + 8) & ~7;
            res->start = (void*)ELF_OFFSET; 

            // Copy the program data into RAM for execution
            uint8 *src = (uint8 *)elfData + phdr->p_offset;
            uint8 *dst = (uint8 *)(ELF_OFFSET + phdr->p_vaddr);
            for (uint32 j = 0; j < phdr->p_filesz; j++)
                dst[j] = src[j];
        }

        phdr = (Elf32_Phdr *)((char *)phdr + ehdr->e_phentsize);
    }

    // Zero the .bss section
    ELF32_shdr *bssSection = elf_find_section(ehdr, ".bss");
    if (bssSection != 0) {
        uint32 *bssArray = (elf32_addr *)(ELF_OFFSET + bssSection->sh_addr);
        elf32_word bssSize = bssSection->sh_size / sizeof(void *);
        for (elf32_word i = 0; i < bssSize; i++)
            bssArray[i] = 0;
    }

    // Fix GOT entries if they exist
    ELF32_shdr *gotSection = elf_find_section(ehdr, ".got");
    if (gotSection != 0) {
        elf32_addr *gotArray = (elf32_addr *)(ELF_OFFSET + gotSection->sh_addr);
        elf32_word gotSize = gotSection->sh_size / sizeof(void *);
        for (elf32_word i = 0; i < gotSize; i++)
            gotArray[i] += ELF_OFFSET;
    }

    // Run any initial constructors
    ELF32_shdr *initArraySection = elf_find_section(ehdr, ".init_array");
    if (initArraySection != 0) {
        elf32_addr *initArray = (elf32_addr *)(ELF_OFFSET +
            initArraySection->sh_addr);
    
        elf32_word initArraySize = initArraySection->sh_size / sizeof(void *);
        for (elf32_word i = 0; i < initArraySize; i++) {
            initArray[i] += ELF_OFFSET;
            ((void (*)(void))(initArray[i]))();
        }
    }

    fclose(fd);
    free(elfData);
    // Return entry point
    //es->entry = (void*)((to_addr + hdr->e_entry) | 0x1);
    restore(q);
    return (ELF_OFFSET + ehdr->e_entry) | 0x1 ;

    //return 0;
}
