/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>

shellcmd xsh_dump(int nargs, char *args[])
{
	 
	FILE* fd;
 
    char *tmp=full_path(args[1]);
    printf("%s\n",tmp );
    if (tmp==NULL)return -1;
    if (!(fd = fopen(tmp,"r"))){
        printf("%s not found\n", tmp);
        return -1;
    }
    uint8 buff[32];
    int part=0;
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    printf("size file %d\n",fileLength);

    while(part<fileLength){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         fread(buff, 32,1,fd);
         hexDump2(part,buff,32);
         memset(buff,0,32);
         part += 32;
    }
    fclose(fd);


    update_path();
	return 0;
}
