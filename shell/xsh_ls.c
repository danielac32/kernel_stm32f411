/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>

 
shellcmd xsh_ls(int nargs, char *args[])
{
	FL_DIR dirstat;
    char *tmp=full_path("");
    if (fl_opendir(tmp, &dirstat))
    {
        struct fs_dir_ent dirent;

        while (fl_readdir(&dirstat, &dirent) == 0)
        {
            if (dirent.is_dir)
            {
                printf("%s <DIR>\r\n", dirent.filename);
            }
            else
            {
                printf("%s [%d bytes]\r\n", dirent.filename, dirent.size);
            }
        }

        fl_closedir(&dirstat);
    }

	return 0;
}
