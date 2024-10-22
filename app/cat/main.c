
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <gpio.h>
#include <syscall.h>
#include <lfs.h>
#include <lfs_util.h>

#define FILE            lfs_file_t
#define DIR             lfs_dir_t
syscall_t syscallp;

int main(int argc, char *argv[])
{
	
	syscall_init(&syscallp);

	//printf("program %s\n",argv[0] );
    //for (int i = 0; i < argc; ++i)
    //{
    //	printf("%s ",argv[i] );
    //}
 
    if(argc <2)return -1;
	if (!strcmp(argv[argc-2],">")){

        char *tmp=syscallp.full_path((char*)argv[argc-1]);
        if (tmp==NULL)return -1;
        printf("-> %s\n",tmp );
	    if(syscallp.exist(tmp)){
           printf("%s found!\n",tmp );
           syscallp.updatepath();//strcpy(path, curdir);
           return 0;
        }

        FILE *fs=syscallp.open(tmp,LFS_O_CREAT);
        if (fs < 0){
            return -1;
        }
        for (int i = 1; i < argc-2; ++i)
        {
        	syscallp.write(argv[i],strlen(argv[i]),fs);
        	syscallp.write(" ",1,fs);
        }
        syscallp.close(fs);
	}else{
		char *tmp=syscallp.full_path((char*)argv[argc-1]);
	    if(!syscallp.exist(tmp)){
           printf("%s not found!\n",tmp );
           syscallp.updatepath();//strcpy(path, curdir);
           return 0;
        }
        FILE *fs=syscallp.open(tmp,LFS_O_RDWR);
        while(syscallp.available(fs)){
        	 char ch;
        	 syscallp.read(&ch,1,fs);
        	 printf("%c",ch );
        }
        syscallp.close(fs);
	}
	syscallp.updatepath();//strcpy(path, curdir);


	return 0;
}