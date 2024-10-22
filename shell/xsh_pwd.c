/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>



shellcmd xsh_pwd(int nargs, char *args[])
{
	char *s=full_path("");
    printf("%s\n",s );
	return 0;
}
