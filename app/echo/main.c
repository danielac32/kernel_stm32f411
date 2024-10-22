
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>

syscall_t syscallp;

int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
   
  int i;

  for (int i = 1; i < argc; ++i)
  {
    printf("%s ", argv[i]);
  }
  printf("\n");

	return 0;
}