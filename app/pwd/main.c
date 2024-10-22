
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <gpio.h>
#include <syscall.h>
#include <elf.h>
#define FILE            lfs_file_t
#define DIR             lfs_dir_t


syscall_t syscallp;

int main(int argc, char const *argv[])
{
	syscall_init(&syscallp);
    int i;
 
    char *s=syscallp.full_path("");
    printf("%s\n",s );

    
	return 0;
}