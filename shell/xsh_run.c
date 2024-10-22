/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>
#include <elf.h>

shellcmd xsh_run(int nargs, char *args[])
{
	char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    FILE *fptr;
    exec_img ximg;
    int32	tmparg;
    int child;

    int ret = elf_execve(tmp,&ximg);
    if(ret > 0){
        int (*p) = (int *)ret;
        child = create(p,
			SHELL_CMDSTK, SHELL_CMDPRIO,
			tmp, 2, nargs, &args[0]);
        
        struct	procent *prptr;
        prptr = &proctab[child];
        prptr->elf = TRUE;
        prptr->img = (void *)ximg.start;

        resume(child);

    }else {
        printf("error loading elf process  %d %s\n",ret,tmp);
    }

    update_path();


	 
	return 0;
}
