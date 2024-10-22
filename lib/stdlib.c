/*
 * Portions of this file were based on source
 * code from Stack Overflow, and fall under the
 * Creative Commons Attribution-ShareAlike 2.5 Generic
 * license.  http://creativecommons.org/licenses/by-sa/2.5/
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


/* labs.c - labs */

/*------------------------------------------------------------------------
 *  labs  -  Calculates the absolute value of a long.
 *------------------------------------------------------------------------
 */
long    labs(
      long      arg
    )
{
    if (arg < 0)
        arg = -arg;
    return (arg);
}

int abs(int n) {
    return n > 0 ? n : -n;
}

char *strndup(const char *str, int n) {
    int len = strnlen(str, n);

    char *dup = malloc(len+1);  // Room for NULL
    if (!dup) {
        return NULL;
    }

    strncpy(dup, str, len);
    dup[len] = '\0';

    return dup;
}

char *strdup(const char *str) {
    return strndup(str, strlen(str));
}

long    atol(
          char      *p
        )
{
    long n;
    int f;

    n = 0;
    f = 0;
    for (;; p++)
    {
        switch (*p)
        {
        case ' ':
        case '\t':
            continue;
        case '-':
            f++;
        case '+':
            p++;
        }
        break;
    }
    while (*p >= '0' && *p <= '9')
    {
        n = n * 10 + *p++ - '0';
    }
    return (f ? -n : n);
}


int atoi(char buf[]) {
    int result = 0;
    int negate = 0;
    int i = 0;

    // ignore leading whitespace
    while (chrnlst(buf[i]," \t\r\n")) {
        i++;
    }

    // check for negative
    if (buf[i] == '-') {
        negate = 1;
        i++;
    }
    else if (buf[i] == '+') {
        i++;
    }

    // parse decimal number
    while (buf[i] >= '0' && buf[i] <= '9') {
        result *= 10;
        result += (buf[i] - '0');
        i++;
    }
    return result * (negate ? -1 : 1);
}

/* Safely takes absolute value of an int,
 * properly handling INT_MIN */
static uint32_t int_abs(int n) {
    if (n > 0) {
        return n;
    }
    else {
        return UINT_MAX - (uint32_t) n + 1;
    }
}

char *itoa(int number, char *buf, uint32_t len, uint32_t base) {
    int i = 0;

    if (!len) {
        return NULL;
    }

    if (base < 2 || base > 36) {
        return NULL;
    }

    if (number < 0) {
        buf[i++] = '-';
        len--;
    }

    number = int_abs(number);

    return uitoa(number, &buf[i], len, base);
}

/* Converts number to string, base 2 to 36.
 * Returns NULL on invalid base or insufficient space */
char *uitoa(uint32_t number, char *buf, uint32_t len, uint32_t base) {
    char lookup[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXY";
    int i = 0;

    if (!len) {
        return NULL;
    }

    if (base < 2 || base > 36) {
        return NULL;
    }

    do {
        buf[i++] = lookup[number % base];
    } while (--len && (number /= base) > 0);

    if (!len) {
        return NULL;
    }

    buf[i] = '\0';

    strreverse(buf);

    return buf;
}

/* Based on http://stackoverflow.com/a/2303798/10817 by Sophy Pal */
void ftoa(float num, float tolerance, char buf[], uint32_t n) {
    int m;
    int digit;

    if (isnan(num)) {
        buf[0%n] = 'n';
        buf[1%n] = 'a';
        buf[2%n] = 'n';
        buf[3%n] = '\0';
        return;
    }
    if (isinf(num)) {
        if (ispos(num)) {
            buf[0%n] = 'i';
            buf[1%n] = 'n';
            buf[2%n] = 'f';
            buf[3%n] = '\0';
            return;
        }
        else {
            buf[0%n] = '-';
            buf[1%n] = 'i';
            buf[2%n] = 'n';
            buf[3%n] = 'f';
            buf[4%n] = '\0';
            return;
        }
    }

    if (num < 0.0f) {
        *(buf++) = '-';
        n--;
        num = -num;
    }

    m = logarithm(num, 1);

    if (m < 0) {
        *(buf++) = '0';
        *(buf++) = '.';
    }

    while ((num > 0 + tolerance || m >= 0) && n > 1) {
        float weight = powf(10.0f, m);
        digit = floorf(num / weight);
        num -= (digit*weight);
        *(buf++)= '0' + digit;
        n--;
        if (m == 0 && n > 1) {
            *(buf++) = '.';
            n--;
        }
        m--;
    }

    /* Print zero after decimal at end */
    if (*(buf-1) == '.' && n > 1) {
        *(buf++) = '0';
        n--;
    }

    *(buf) = '\0';
}



void    bzero(
      void      *p,
      int       len
    )
{
    int n;
    char *pch = (char *)p;

    if ((n = len) <= 0)
    {
        return;
    }
    do
    {
        *pch++ = 0;
    }
    while (--n);
}
