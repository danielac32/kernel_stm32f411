
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

/*
int replacestr(char *line, const char *search, const char *replace)
{
   int count;
   char *sp; // start of pattern

   //printf("replacestr(%s, %s, %s)\n", line, search, replace);
   if ((sp = strstr(line, search)) == NULL) {
      return(0);
   }
   count = 1;
   int sLen = strlen(search);
   int rLen = strlen(replace);
   if (sLen > rLen) {
      // move from right to left
      char *src = sp + sLen;
      char *dst = sp + rLen;
      while((*dst = *src) != '\0') { dst++; src++; }
   } else if (sLen < rLen) {
      // move from left to right
      int tLen = strlen(sp) - sLen;
      char *stop = sp + rLen;
      char *src = sp + sLen + tLen;
      char *dst = sp + rLen + tLen;
      while(dst >= stop) { *dst = *src; dst--; src--; }
   }
   memcpy(sp, replace, rLen);

   count += replacestr(sp + rLen, search, replace);

   return(count);
}
*/

struct   cmdent   {        /* Entry in command table  */
   char  cname[20];        /* Name of command      */
   //uint32 stack;

};

const struct   cmdent   cmdtab[] = {
   //{"shell/help.elf"},
   {"shell/ls.elf"},
   //{"bin/app2.elf"},
   {"shell/mem.elf"},
   {"shell/ps.elf"},
   {"bin/blink.elf"},
   {"shell/shell.elf"},
};


int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
   #if 0
   printf("init process\n");
    if(!syscallp.exist("shell/shell.json")){
      printf("error init shell\n");
      return -1;
    }
    FILE *fs=syscallp.open("shell/shell.json",LFS_O_RDONLY);

	int size = syscallp.size(fs);
	char *jsonbuff=syscallp.malloc(size+1);
	syscallp.read(jsonbuff,size,fs);
	syscallp.close(fs);
    char *val;

    int len;
    int vlen = 0;
    

    val = syscallp.js0n("load",0,jsonbuff,size,&len);
    char *p = syscallp.malloc(len+1);
    sprintf(p,"%.*s \n",(int)len,val);

    //printf("%s\n",p );

    replacestr(p,"	","");
    replacestr(p,"[","");
    replacestr(p,"]","");
    replacestr(p,"\"","");
    replacestr(p,"\n","");
    replacestr(p," ","");
 
    //printf("%s\n",p );
	
	char *token;
    exec_img ximg;
    int ret;
	void (*func)(void);


   /* get the first token */
   token = strtok(p, " ,");
   
   /* walk through other tokens */
   while( token != NULL ) {
       printf( "%s\n", token );

       ret = syscallp.elf_execve(token,&ximg);
       if(ret == -1){
          printf("error load %s\n", token);
		  return -1;
       }
       func = (void *)ret;
       int child = syscallp.create(func, 4096, 3, token);
       task_t *prptr=syscallp.self(child);	
       prptr->elf = TRUE;
       prptr->img = (void *)ximg.start;
	    syscallp.ready(child);
       syscallp.sleep(1000);
       token = strtok(NULL, " ,");
   }
    
   syscallp.free(p);
   #endif 
   
   exec_img ximg;
   int ret;
   int (*func);


   for (int i = 0; i < sizeof(cmdtab) / sizeof(struct cmdent); ++i)
   {
      printf("%s\n",cmdtab[i].cname );
      ret = syscallp.elf_execve(cmdtab[i].cname,&ximg);
       if(ret == -1){
          printf("error load %s\n", cmdtab[i].cname);
        return -1;
       }
       func = (int *)ret;
       int child = syscallp.create((void *)func, 2048, 3, cmdtab[i].cname);
       task_t *prptr=syscallp.self(child);   
       prptr->elf = TRUE;
       prptr->img = (void *)ximg.start;
       syscallp.ready(child);
       syscallp.sleep(100);
   }
    
    
   return 0;
}