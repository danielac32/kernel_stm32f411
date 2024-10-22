//#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//#include <stm32.h>
//#include <queue.h>
//#include <memory.h>
#include <kernel.h>
//#include <littlefs.h>
#include <process.h>
#include <syscall.h>
#include <elf.h>

syscall_t *sys;

void* sys_open(const char *path, int flags){
    return (void *)__syscall(XINU_FOPEN,path,flags);
}
int sys_exist(const char *path){
    return __syscall(XINU_EXIST,path);
}
int sys_write(void *pbuf, int size, void *file){
    return __syscall(XINU_FWRITE,pbuf,size,file);
}
int sys_read(void *pbuf, int size, void *file){
    return __syscall(XINU_FREAD,pbuf,size,file);
}
int sys_close(void *file){
    return __syscall(XINU_FCLOSE,file);
}
int sys_seek(void *file, int offset, int whence){
    return __syscall(XINU_FSEEK,file,offset,whence);
}
int sys_tell(void *file){
    return __syscall(XINU_FTELL,file);
}
int sys_rewind(void *file){
    return __syscall(XINU_FREWIN,file);
}
int sys_truncate(int size, void *file){
    return __syscall(XINU_TRUNCATE,size,file);
}
int sys_size(void *file){
    return __syscall(XINU_SIZE,file);
}
int sys_available(void *file){
    return __syscall(XINU_AVAILABLE,file);
}

int sys_mkdir(const char *path){
    return __syscall(XINU_MKDIR,path);
}
void *sys_openDir(const char *path){
    return (void *)__syscall(XINU_OPENDIR,path);
}
int sys_closeDir(void *dir){
    return __syscall(XINU_CLOSEDIR,dir);
}
int sys_readDir(void *dir, void *info){
    return __syscall(XINU_READDIR,dir,info);
}
int sys_rewindDir(void *dir){
    return 0;
}
int sys_tellDir(void *dir){
    return 0;
}
int sys_seekDir(void *dir, int offset){
    return 0;
}

int sys_format(void *_lfs, void *_cfg){
    return __syscall(XINU_FORMAT,_lfs,_cfg);
}
int sys_mount(void *_lfs, void *_cfg){
    return __syscall(XINU_MOUNT,_lfs,_cfg);
}
int sys_unmount(void *_lfs){
    return __syscall(XINU_UNMOUNT,_lfs);
}
int sys_remove(const char *path){
    return __syscall(XINU_REMOVE,path);
}
int sys_rename(const char *oldpath, const char *newpath){
    return __syscall(XINU_RENAME,oldpath,newpath);
}
int sys_free(void){
    return __syscall(XINU_DISK_FREE);
}
void sys_sleep(uint32 ticks){
    __syscall(XINU_SLEEP,ticks);
}

void sys_kill(int pid){
    __syscall(XINU_KILL,pid);
}

int sys_getpid(){
    return __syscall(XINU_GETPID);
}

void sys_yield(){
    __syscall(XINU_YIELD);
}

void sys_ready(int pid){
    __syscall(XINU_READY,pid);
}

char *sys_full_path(char *s){
    return (char *)__syscall(XINU_GLOBAL_PATH,s);
}
void sys_update_path(){
    __syscall(XINU_UPDATE_PATH);
}

void *sys_malloc(uint32 size){
    return (void *)__syscall(XINU_MALLOC,size);
}

void sys_mfree(void *p){
   __syscall(XINU_FREE,p);
}

void sys_putc(char c){
    __syscall(XINU_PUTC,c);
}
void sys_puts(char *s){
    __syscall(XINU_PUTS,s);
}

void *sys_struct_task(int pid){
return (void *)__syscall(XINU_STRUCT_TASK,pid);
}

char *sys_get_path(){
    return (char *)__syscall(XINU_GET_PATH);
}

void sys_cd(char *s){
    __syscall(XINU_CD,s);
}

uint32 sys_elf_execve(const char *file, void *res){
       return __syscall(XINU_LOAD_ELF,file,res);
}


int sys_create(void *procaddr, uint32 ssize, int priority,const char *name){
    return __syscall(XINU_CREATE,procaddr,ssize,name);
}

//const char *sys_js0n(const char *key, size_t klen,const char *json, size_t jlen, size_t *vlen){
  //  return (char *)__syscall(XINU_JSON,key,klen,json,jlen,vlen);
//}
void *sys_js0n(void *frame){
    return __syscall(XINU_JSON,frame);
}

uint32 sys_free_heap(){
    return __syscall(XINU_FREE_HEAP);
}

int syscall_init(syscall_t *sys_obj){
    sys = sys_obj;
    sys->exist = sys_exist;
    sys->open = sys_open;
    sys->write = sys_write;
    sys->read = sys_read;
    sys->close = sys_close;
    sys->seek = sys_seek;
    sys->tell = sys_tell;
    sys->rewind = sys_rewind;
    sys->truncate = sys_truncate;
    sys->size = sys_size;
    sys->available = sys_available;

    sys->mkdir = sys_mkdir;
    sys->openDir = sys_openDir;
    sys->closeDir = sys_closeDir;
    sys->readDir = sys_readDir;
    sys->rewindDir = sys_rewindDir;
    sys->tellDir = sys_tellDir;
    sys->seekDir = sys_seekDir;

    sys->format = sys_format;
    sys->mount = sys_mount;
    sys->unmount = sys_unmount;
    sys->remove = sys_remove;
    sys->rename = sys_rename;
    sys->disk_free = sys_free;
    sys->sleep = sys_sleep;
    sys->kill = sys_kill;
    sys->getpid = sys_getpid;
    sys->yield = sys_yield;
    sys->ready = sys_ready;
    //sys->create = 0;
    sys->reset = 0;
    sys->putc = sys_putc;
    sys->puts = sys_puts;
    sys->kbhit = 0;
    sys->readbyte = 0;
    sys->full_path = sys_full_path;
    sys->updatepath = sys_update_path;
    sys->malloc = sys_malloc;
    sys->free = sys_mfree;
    sys->self = sys_struct_task;
    sys->getpath = sys_get_path;
    sys->cd = sys_cd;
    sys->elf_execve=sys_elf_execve;
    sys->create = sys_create;
    sys->js0n = sys_js0n;
    sys->freeHeap = sys_free_heap;
    return 0;
}
