/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>



shellcmd xsh_touch(int nargs, char *args[])
{
	FILE* fd;
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    if ((fd = fopen(tmp,"r"))){
        printf("file found %s\n",tmp);
        fclose(fd);
        return -1;
    }
    fd = fopen(tmp,"w");
    fclose(fd);
	return 0;
}
