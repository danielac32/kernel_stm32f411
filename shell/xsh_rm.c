/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>


shellcmd xsh_rm(int nargs, char *args[])
{
	FILE* fd;
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    if (!(fd = fopen(tmp,"r"))){
        printf("not found %s\n",tmp);
        return -1;
    }
    fclose(fd);
    if(remove(tmp)<0){
       printf("error remove \n");
    }
	return 0;
}
