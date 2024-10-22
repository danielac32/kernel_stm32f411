
#ifndef _FS_H_
#define _FS_H_

#include <stdio.h>
#include <kernel.h>
#include <riscv.h>
#include <stdlib.h>
#include <string.h>
#include <lfs.h>
#include <lfs_util.h>



#define FILE            lfs_file_t
#define DIR 			lfs_dir_t


typedef struct littlefs_s
{
	lfs_t *lfs;
    lfs_file_t *(*open)(const char *path, int flags);
    int (*write)(void *pbuf, int size, lfs_file_t *file);
    int (*read)(void *pbuf, int size, lfs_file_t *file);
    int (*close)(lfs_file_t *file);
    int (*seek)(lfs_file_t *file, int offset, int whence);
    int (*tell)(lfs_file_t *file);
    int (*rewind)(lfs_file_t *file);
    int (*truncate)(int size, lfs_file_t *file);
    int (*size)(lfs_file_t *file);
    int (*available)(lfs_file_t *file);

    int (*mkdir)(const char *path);
    lfs_dir_t *(*openDir)(const char *path);
    int (*closeDir)(lfs_dir_t *dir);
    int (*readDir)(lfs_dir_t *dir, struct lfs_info *info);
    int (*rewindDir)(lfs_dir_t *dir);
    int (*tellDir)(lfs_dir_t *dir);
    int (*seekDir)(lfs_dir_t *dir, int offset);


    int (*format)(lfs_t *_lfs, const struct lfs_config *_cfg);
    int (*mount)(lfs_t *_lfs, const struct lfs_config *_cfg);
    int (*unmount)(lfs_t *_lfs);
    int (*remove)(const char *path);
    int (*rename)(const char *oldpath, const char *newpath);
    int (*free)(void);
    int (*tree)(const char *dir_path, char *pbuf, int buf_size);
    
    int (*exist)(const char *path);
} littlefs_t;
 

extern littlefs_t *littlefs;
int littlefs_init(littlefs_t *lfs_obj, struct lfs_config *config,int size);
extern littlefs_t disk;
extern struct lfs_config config;




/*int                 rmdir(const char *path);

int                 mkdir(const char *path);
DIR*                opendir(const char* path, DIR *dir);
int                 readdir(DIR *dirls, dirent *entry);
int                 closedir(DIR* dir);
void*               fopen(const char *path, const char *modifiers);
void                fclose(void *file);
int                 fflush(void *file);
int                 fgetc(void *file);
char *              fgets(char *s, int n, void *f);
int                 fputc(int c, void *file);
int                 fputs(const char * str, void *file);
int                 fwrite(const void * data, int size, int count, void *file );
int                 fread(void * data, int size, int count, void *file );
int                 fseek(void *file , long offset , int origin );
int                 fgetpos(void *file , uint32 * position);
long                ftell(void *f);
int                 feof(void *f);
int                 remove(const char * filename);*/


#endif