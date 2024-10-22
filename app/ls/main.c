
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
	struct lfs_info info;

	syscall_init(&syscallp);
    //printf("aquiiiiiiiii\n" );
    char *s=syscallp.getpath();

    if (s==NULL || !strcmp(s,"")) return -1;
    DIR *dir = syscallp.openDir(s);
    if(dir==0){
        printf("not a directory %s",s);
        return -1;
    }
    while (syscallp.readDir(dir, &info)> 0){
        if (strcmp(info.name, ".") && strcmp(info.name, "..") && strcmp(info.name, "//")){            
            char *tmp=syscallp.full_path(info.name);
            if (tmp==NULL) return -1;
            if(syscallp.exist(tmp)){
                if (info.type == LFS_TYPE_DIR){
                    printf(" %s\n",info.name);
                }else{
                    printf(" %s %d \n",info.name,info.size);
                }
            }
            syscallp.updatepath();
        }
    }

    syscallp.closeDir(dir);

	return 0;
}