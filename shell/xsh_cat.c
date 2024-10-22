/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>



shellcmd xsh_cat(int nargs, char *args[])
{
	 
     FILE* fd;

	/* For argument '--help', emit help about the 'cat' command	*/
    

 
   /*	if (nargs == 2 && strcmp(args[1], "h") == 0) {
   		//cat 
		printf("Use: %s [file...]\n\n", args[0]);
		printf("Description:\n");
		printf("\twrites contents of files or stdin to stdout\n");
		printf("Options:\n");
		printf("\tfile...\tzero or more file names\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}*/
    
    
    if(nargs <2)return -1;

    if (!strcmp(args[nargs-2],">")){
    	char *tmp=full_path((char*)args[nargs-1]);
    	if (tmp==NULL)return -1;

    	fd = fopen(tmp, "w");
        if(!fd){
           printf("%s found!\n",tmp );
           goto done;//syscallp.updatepath();//update_path();//strcpy(path, curdir);
        }
        for (int i = 1; i < nargs-2; ++i)
        {
            fputs(args[i],fd);
            fputs(" ",fd);
        }
    }else{
 		char *tmp=full_path((char*)args[nargs-1]);
 		if (tmp==NULL)return -1;
	    if (!(fd = fopen(tmp,"r"))){
            printf("%s not found\n", tmp);
            goto done;
        }
        char buff[64];
        while(!feof(fd)){
              char c = fgetc(fd);
              putc(CONSOLE,c);
        }
    }
    done:
    printf("\n");
    fclose(fd);
	update_path();
	return 0;
}
