#include <os.h>

syscall_t *sys;

void* sys_open(const char *path, int flags){
    return (void *)syscall(XINU_FOPEN,path,flags);
}
int sys_exist(const char *path){
    return syscall(XINU_EXIST,path);
}
int sys_write(void *pbuf, int size, void *file){
    return syscall(XINU_FWRITE,pbuf,size,file);
}
int sys_read(void *pbuf, int size, void *file){
    return syscall(XINU_FREAD,pbuf,size,file);
}
int sys_close(void *file){
    return syscall(XINU_FCLOSE,file);
}
int sys_seek(void *file, int offset, int whence){
    return syscall(XINU_FSEEK,file,offset,whence);
}
int sys_tell(void *file){
    return syscall(XINU_FTELL,file);
}
int sys_rewind(void *file){
    return syscall(XINU_FREWIN,file);
}
int sys_truncate(int size, void *file){
    return syscall(XINU_TRUNCATE,size,file);
}
int sys_size(void *file){
    return syscall(XINU_SIZE,file);
}
int sys_available(void *file){
    return syscall(XINU_AVAILABLE,file);
}

int sys_mkdir(const char *path){
    return syscall(XINU_MKDIR,path);
}
void *sys_openDir(const char *path){
    return (void *)syscall(XINU_OPENDIR,path);
}
int sys_closeDir(void *dir){
    return syscall(XINU_CLOSEDIR,dir);
}
int sys_readDir(void *dir, void *info){
    return syscall(XINU_READDIR,dir,info);
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
    return syscall(XINU_FORMAT,_lfs,_cfg);
}
int sys_mount(void *_lfs, void *_cfg){
    return syscall(XINU_MOUNT,_lfs,_cfg);
}
int sys_unmount(void *_lfs){
    return syscall(XINU_UNMOUNT,_lfs);
}
int sys_remove(const char *path){
    return syscall(XINU_REMOVE,path);
}
int sys_rename(const char *oldpath, const char *newpath){
    return syscall(XINU_RENAME,oldpath,newpath);
}
int sys_free(void){
    return syscall(XINU_DISK_FREE);
}
void sys_sleep(uint32 ticks){
    syscall(XINU_SLEEP,ticks);
}

void sys_kill(int pid){
    syscall(XINU_KILL,pid);
}

int sys_getpid(){
    return syscall(XINU_GETPID);
}

void sys_yield(){
    syscall(XINU_YIELD);
}

void sys_ready(int pid){
    syscall(XINU_READY,pid);
}

char *sys_full_path(char *s){
    return (char *)syscall(XINU_GLOBAL_PATH,s);
}
void sys_update_path(){
    syscall(XINU_UPDATE_PATH);
}

void *sys_malloc(uint32 size){
    return (void *)syscall(XINU_MALLOC,size);
}

void sys_mfree(void *p){
     syscall(XINU_FREE,p);
}

void sys_putc(char c){
    syscall(XINU_PUTC,c);
}
void sys_puts(char *s){
    syscall(XINU_PUTS,s);
}

void *sys_struct_task(int pid){
return (void *)syscall(XINU_STRUCT_TASK,pid);
}

char *sys_get_path(){
    return (char *)syscall(XINU_GET_PATH);
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
    sys->create = 0;
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
    return 0;
}
