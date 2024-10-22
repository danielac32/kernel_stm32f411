

#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <gpio.h>
#include <syscall.h>

syscall_t syscallp;

int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
	if (argc < 2)return -1;
   
	char *tmp=syscallp.full_path(argv[1]);
    syscallp.cd(tmp);
    syscallp.updatepath();

	return 0;
}