
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

int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
	uint32 memfree=syscallp.freeHeap();
	//uint32 fsfree=syscallp.disk_free();

    printf("mem free: %d\n", memfree);
    //printf("fs free: %d\n", fsfree);
    //printf("ok\n");
	return 0;
}