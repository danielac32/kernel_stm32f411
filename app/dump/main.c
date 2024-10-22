
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

int main(int argc, char *args[])
{
	syscall_init(&syscallp);
    char *tmp=syscallp.full_path(args[1]);
    if (tmp==NULL)return -1;
	if(!syscallp.exist(tmp)){
       printf("%s not found!\n",args[1] );
       syscallp.updatepath();
       return -1;
    }
    uint8 buff[32];
    //buff=syscallp.malloc(32);
    //char buff[32];
    int part=0;
	FILE *fs=syscallp.open(tmp,LFS_O_RDWR);
	int size = syscallp.size(fs);
	printf("size file %d\n",size);
	while(part<size){
		 //syscallp.seek(fs,part,LFS_SEEK_SET);
         syscallp.read(buff,32,fs);
         hexDump2(part,buff,32);
         memset(buff,0,32);
         part += 32;
	}
    syscallp.close(fs);
    //syscallp.free(buff);


    syscallp.updatepath();

	return 0;
}