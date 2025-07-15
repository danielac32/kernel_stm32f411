/* sprintf.c - sprintf */
#include <xinu.h>
//#include <stdarg.h>
//#include <stdlib.h>
//#include <stdio.h>
static int sprntf(int, int);
extern void _fdoprnt(char *, va_list, int (*func) (int, int), int);

/*------------------------------------------------------------------------
 *  sprintf  -  Format arguments and place output in a string.
 *------------------------------------------------------------------------
 */
int	sprintf(
	  char		*str,		/* output string		*/
	  char		*fmt,		/* format string		*/
	  ...
	)
{
    va_list ap;
    char *s;

    s = str;
    va_start(ap, fmt);
    _fdoprnt(fmt, ap, sprntf, (int)&s);
    va_end(ap);
    *s++ = '\0';

    return ((int)str);
}



/*------------------------------------------------------------------------
 *  sprntf  -  Routine called by _doprnt to handle each character.
 *------------------------------------------------------------------------
 */
static int	sprntf(
		  int		acpp,
		  int		ac
		)
{
    char **cpp = (char **)acpp;
    char c = (char)ac;

    return (*(*cpp)++ = c);
}


int snprintf(
    char *str,       // Buffer de salida
    int size,     // Tamaño máximo del buffer
    char *fmt, // Cadena de formato
    ...              // Argumentos variables
)
{
    va_list ap;
    char *s;
    int ret = 0;

    // Verifica que el buffer y el tamaño sean válidos
    if (str == NULL || size == 0) {
        return -1; // Error: buffer inválido
    }

    s = str;
    va_start(ap, fmt);

    // Llama a _fdoprnt para formatear la cadena
    _fdoprnt(fmt, ap, sprntf, (int)&s);

    va_end(ap);

    // Calcula el número de caracteres escritos
    ret = (int)(s - str);

    // Asegúrate de no exceder el tamaño del buffer
    if ((size_t)ret >= size) {
        // Si se excede, trunca la cadena y agrega el carácter nulo
        str[size - 1] = '\0';
        ret = size - 1; // Retorna el número de caracteres que se habrían escrito
    } else {
        // Si no se excede, agrega el carácter nulo al final
        *s = '\0';
    }

    return ret; // Retorna el número de caracteres escritos (sin contar el nulo)
}