
#ifndef __SYSCALL_H__
#define __SYSCALL_H__
#include <lfs.h>
#include <lfs_util.h>
extern int syscall(int a,...);
enum {
XINU_NULLPROCESS = 0,

XINU_PUTC ,
XINU_PUTS ,
XINU_GETC,
XINU_GETS,
    //XINU_INIT,
XINU_CLOSE,  /* Devices */
XINU_OPEN ,
XINU_READ ,
XINU_WRITE ,

XINU_CREATE , /* Processes */ 
    //XINU_CHPRIO,
XINU_KILL, /* Exit */
    //XINU_GETPRIO,
XINU_READY ,

XINU_SLEEP ,
XINU_FREE ,
XINU_MALLOC, 

XINU_GETDEV ,

XINU_SEMA_INIT,  /* Semapho3 */
XINU_SEMA_SIGNAL, 
XINU_SEMA_WAIT ,


XINU_PIN_MODE ,
XINU_PIN_SET ,
XINU_PIN_GET ,
XINU_KMALLOC_CHUNK, 
XINU_GETPID ,

XINU_EXIST ,
XINU_FS_INIT ,
XINU_ATTACH_MEDIA, 
XINU_ATTACH_LOCKS , //XINU_LIST_DIR 25
XINU_FOPEN,
XINU_FSEEK,
XINU_FREAD,
XINU_FWRITE,
XINU_FCLOSE,
XINU_FTELL,
XINU_FREWIN,
XINU_TRUNCATE,
XINU_SIZE,
XINU_AVAILABLE,
XINU_MKDIR,
XINU_FEOF,

XINU_OPENDIR,
XINU_READDIR,

XINU_REMOVE,
XINU_RENAME,
XINU_FORMAT,
XINU_MOUNT,
XINU_UNMOUNT,
XINU_DISK_FREE,

XINU_CLOSEDIR,
XINU_CREATEDIRECTORY,
XINU_IS_DIR,
XINU_FFLUSH,
XINU_FGETC,
XINU_FGETCS,
XINU_FPUTC,
XINU_FPUTCS,
XINU_FGETPOS,
XINU_NUM_TASK ,
XINU_STRUCT_TASK, 
XINU_LOAD_ELF,
XINU_HEAP_FREE,
XINU_YIELD,
XINU_GLOBAL_PATH,
XINU_UPDATE_PATH

};



typedef struct syscall_s
{
    void *(*open)(const char *path, int flags);
    int (*write)(void *pbuf, int size, void *file);
    int (*read)(void *pbuf, int size, void *file);
    int (*close)(void *file);
    int (*seek)(void *file, int offset, int whence);
    int (*tell)(void *file);
    int (*rewind)(void *file);
    int (*truncate)(int size, void *file);
    int (*size)(void *file);
    int (*available)(void *file);

    int (*mkdir)(const char *path);
    void *(*openDir)(const char *path);
    int (*closeDir)(void *dir);
    int (*readDir)(void *dir, void *info);
    int (*rewindDir)(void *dir);
    int (*tellDir)(void *dir);
    int (*seekDir)(void *dir, int offset);


    int (*format)(void *_lfs, void *_cfg);
    int (*mount)(void *_lfs, void *_cfg);
    int (*unmount)(void *_lfs);
    int (*remove)(const char *path);
    int (*rename)(const char *oldpath, const char *newpath);
    int (*disk_free)(void);
    int (*tree)(const char *dir_path, char *pbuf, int buf_size);
    
    int (*exist)(const char *path);

    void (*sleep)(uint32);
    void (*kill)(int);
    int (*getpid)();
    void (*yield)();
    int (*ready)(int);
    int (*create)(void *procaddr, uint32 ssize, int priority,const char *name, int nargs, ...);
    void (*reset)();//SYSCON = 0x5555; 
    void (*putc)(char );
    void (*puts)(char *);
    int (*kbhit)();
    int (*readbyte)();
    char *(*full_path)(char *);
    void (*updatepath)();
    void *(*malloc)(uint32);
    void (*free)(void *);
    void *(*self)(int);
    
}syscall_t;
extern syscall_t *sys;
extern syscall_t syscallp;


int syscall_init(syscall_t *sys_obj);
#endif


