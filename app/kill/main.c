#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <gpio.h>
#include <syscall.h>
#define FILE            lfs_file_t
#define DIR             lfs_dir_t

syscall_t syscallp;




int atoi(
      char      *p
    )
{
    int n = 0, f = 0;

    for (;; p++)
    {
        switch (*p)
        {
        case ' ':
        case '\t':
            continue;
        case '-':
            f++;
        case '+':
            p++;
        }
        break;
    }

    while (*p >= '0' && *p <= '9')
    {
        n = n * 10 + *p++ - '0';
    }

    return (f ? -n : n);
}

int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
    int i;

  if(argc < 2){
    printf("usage: kill pid...\n");
    return -1;
  }
  for(i=1; i<argc; i++)
    syscallp.kill(atoi(argv[i]));
    
	return 0;
}