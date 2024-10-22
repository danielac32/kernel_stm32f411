/* Xinu system calls */
extern int __syscall(int a,...);
enum {
XINU_NULLPROCESS = 0,
XINU_PUTC ,
XINU_PUTS ,
XINU_GETC,
XINU_GETS,
XINU_CLOSE,  /* Devices */
XINU_OPEN ,
XINU_READ ,
XINU_WRITE ,
XINU_CREATE , /* Processes */ 
XINU_KILL, /* Exit */
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
XINU_FREE_HEAP, 
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
XINU_UPDATE_PATH,
XINU_GET_PATH,
XINU_CD,
XINU_JSON,
XINU_GET_LEN,


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
    void (*ready)(int);
    //int (*create)(void *procaddr, uint32 ssize, int priority,const char *name, int nargs, ...);
    void (*reset)();//SYSCON = 0x5555; 
    int (*putc)(int ,int );
    void (*puts)(int ,char *,int);
    int (*kbhit)();
    int (*readbyte)();
    char *(*full_path)(char *);
    void (*updatepath)();
    void *(*malloc)(uint32);
    void (*free)(void *);
    void *(*self)(int);
    char *(*getpath)();
    void (*cd)(char *);
    uint32 (*freeHeap)();
    uint32 (*elf_execve)(const char *file, void *res);
    int (*create)(void *procaddr, uint32 ssize, int priority,const char *name);
    void *(*js0n)(void *);////const char *(*js0n)(const char *key, size_t klen,const char *json, size_t jlen, size_t *vlen);
    uint32 (*len)();
    char *(*fifo)();
}syscall_t;
extern syscall_t *sys;
extern syscall_t syscallp;



int syscall_init(syscall_t *sys_obj);
typedef void *(*syscall_handler_t)(uint32 *sp);

extern void *SVC_XINU_NULLPROCESS(uint32 *);
extern void *SVC_XINU_PUTC (uint32 *);
extern void *SVC_XINU_PUTS (uint32 *);
extern void *SVC_XINU_GETC(uint32 *);
extern void *SVC_XINU_GETS(uint32 *);
extern void *SVC_XINU_CLOSE(uint32 *);  /* Devices */
extern void *SVC_XINU_OPEN (uint32 *);
extern void *SVC_XINU_READ (uint32 *);
extern void *SVC_XINU_WRITE (uint32 *);
extern void *SVC_XINU_CREATE (uint32 *); /* Processes */ 
extern void *SVC_XINU_KILL(uint32 *); /* Exit */
extern void *SVC_XINU_READY (uint32 *);
extern void *SVC_XINU_SLEEP (uint32 *);
extern void *SVC_XINU_FREE (uint32 *);
extern void *SVC_XINU_MALLOC(uint32 *); 
extern void *SVC_XINU_GETDEV (uint32 *);
extern void *SVC_XINU_SEMA_INIT(uint32 *);  /* Semapho3 */
extern void *SVC_XINU_SEMA_SIGNAL(uint32 *); 
extern void *SVC_XINU_SEMA_WAIT (uint32 *);
extern void *SVC_XINU_PIN_MODE (uint32 *);
extern void *SVC_XINU_PIN_SET (uint32 *);
extern void *SVC_XINU_PIN_GET (uint32 *);
extern void *SVC_XINU_FREE_HEAP(uint32 *); 
extern void *SVC_XINU_GETPID (uint32 *);
extern void *SVC_XINU_EXIST (uint32 *);
extern void *SVC_XINU_FS_INIT (uint32 *);
extern void *SVC_XINU_ATTACH_MEDIA(uint32 *); 
extern void *SVC_XINU_ATTACH_LOCKS (uint32 *); //extern void *SVC_XINU_LIST_DIR 25
extern void *SVC_XINU_FOPEN(uint32 *);
extern void *SVC_XINU_FSEEK(uint32 *);
extern void *SVC_XINU_FREAD(uint32 *);
extern void *SVC_XINU_FWRITE(uint32 *);
extern void *SVC_XINU_FCLOSE(uint32 *);
extern void *SVC_XINU_FTELL(uint32 *);
extern void *SVC_XINU_FREWIN(uint32 *);
extern void *SVC_XINU_TRUNCATE(uint32 *);
extern void *SVC_XINU_SIZE(uint32 *);
extern void *SVC_XINU_AVAILABLE(uint32 *);
extern void *SVC_XINU_MKDIR(uint32 *);
extern void *SVC_XINU_FEOF(uint32 *);
extern void *SVC_XINU_OPENDIR(uint32 *);
extern void *SVC_XINU_READDIR(uint32 *);
extern void *SVC_XINU_REMOVE(uint32 *);
extern void *SVC_XINU_RENAME(uint32 *);
extern void *SVC_XINU_FORMAT(uint32 *);
extern void *SVC_XINU_MOUNT(uint32 *);
extern void *SVC_XINU_UNMOUNT(uint32 *);
extern void *SVC_XINU_DISK_FREE(uint32 *);
extern void *SVC_XINU_CLOSEDIR(uint32 *);
extern void *SVC_XINU_CREATEDIRECTORY(uint32 *);
extern void *SVC_XINU_IS_DIR(uint32 *);
extern void *SVC_XINU_FFLUSH(uint32 *);
extern void *SVC_XINU_FGETC(uint32 *);
extern void *SVC_XINU_FGETCS(uint32 *);
extern void *SVC_XINU_FPUTC(uint32 *);
extern void *SVC_XINU_FPUTCS(uint32 *);
extern void *SVC_XINU_FGETPOS(uint32 *);
extern void *SVC_XINU_NUM_TASK (uint32 *);
extern void *SVC_XINU_STRUCT_TASK(uint32 *); 
extern void *SVC_XINU_LOAD_ELF(uint32 *);
extern void *SVC_XINU_HEAP_FREE(uint32 *);
extern void *SVC_XINU_YIELD(uint32 *);
extern void *SVC_XINU_GLOBAL_PATH(uint32 *);
extern void *SVC_XINU_UPDATE_PATH(uint32 *);
extern void *SVC_XINU_GET_PATH(uint32 *);
extern void *SVC_XINU_CD(uint32 *);
extern void *SVC_XINU_JSON(uint32 *);
extern void *SVC_XINU_GET_LEN(uint32 *);

