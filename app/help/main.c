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


int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
  struct lfs_info info;
  char s[]={"shell"};

  if (s==NULL || !strcmp(s,"")) return -1;
  DIR *dir = syscallp.openDir(s);
  if(dir==0){
      printf("not a directory %s",s);
      return -1;
  }

  char *p=syscallp.malloc(100);
  while (syscallp.readDir(dir, &info)> 0){
      if (strcmp(info.name, ".") && strcmp(info.name, "..") && strcmp(info.name, "//") && strcmp(info.name, "shell.json")){            
          if (info.type != LFS_TYPE_DIR){
              //printf(" %s \n",info.name);
              strcpy(p,info.name);
              replacestr(p,".elf","");
              printf(" %s \n",p);
          }
      }
  }

  syscallp.closeDir(dir);
  syscallp.free(p);


	return 0;
}