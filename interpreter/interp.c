/**
 * @file shell.c
 * File loader to test scripts in host environment
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 
#include <xinu.h>
#include <fat_filelib.h>
#include "parser.h"
//#include <shell.h>
int print(instance *it)
{
	variable *s = igetarg(it, 0);
	if (s->type == NUMBER) {
		if (s->value.f == (int)s->value.f)
			printf("%d\n", (int)s->value.f);
		else
			printf("float: %d\n", s->value.f);
	} else if (s->value.p != 0) {
		printf("%s\n", (char *)s->value.p);
	}
	return 0;
}
/*
int gets(instance *it)
{
	char *line = 0;
	size_t size;
	getline(&line, &size, stdin);
	*strchr(line, '\n') = '\0';
	variable *v = make_vars(0, line);
	free(line);
	ipush(it, (size_t)v);
	return 0;
}*/

extern uint32 heap_free();
 char *code =
    "# test1\n"
    "# arithmetic tests\n"
    "# looking for proper basic function, respect for order of ops,\n"
    "# and respect for parentheses\n"
    "\n"
    "print(2 + 5)\n"
    "print(14 - 9)\n"
    "print(3 * 8 + 3)\n"
    "print(9 - 3 / 2)\n"
    "print(3 * (8 + 3))\n"
    "print((9 - 3) / 2)\n"
    "print((4 + 5) * ((9 - 1) + 3))\n"
    "print(5 - 3 + 4)\n"
    "print(\"\")\n"
    "print(-4)\n"
    "print(-4 + -3)\n"
    "print(-8+13)\n"
    "print(4- -9)\n";


char line[256];

int interp(int argc, char *argv[])
{
	/*if (argc != 2) {
		printf("Usage: %s file\n", argv[0]);
		return -1;
	}

    
    char *tmp=full_path((char*)argv[1]);
    if (tmp==NULL)return -1;

	FILE *fp = fopen(tmp, "r");
	if (fp == 0) {
		printf("Could not open file: %s\n", argv[1]);
		return -1;
	}*/
    printf("mem: %d\n", heap_free());
	instance *it = inewinstance();
	inew_cfunc(it, "print", print);
	//inew_cfunc(it, "gets", gets);

	/*char *line = 0;
	size_t size;
	int result;



	while(!feof(fp)){
			fgets(line, size, fp);
			result = iaddline(it, line);
			if (result != 0)
				printf("Error: %d\n", result);
	}*/




     // Buffer para almacenar cada línea
    char *ptr = code; // Puntero para recorrer el string
    int result;

    while (*ptr != '\0') { // Mientras no lleguemos al final del string
        // Copiar una línea en el buffer
        int i = 0;
        while (*ptr != '\n' && *ptr != '\0') {
            line[i++] = *ptr++;
        }
        line[i] = '\0'; // Terminar la línea con un carácter nulo

        // Avanzar el puntero si no hemos llegado al final
        if (*ptr == '\n') {
            ptr++;
        }

        // Procesar la línea (aquí puedes llamar a iaddline o hacer lo que necesites)
        result = iaddline(it, line); // Reemplaza `it` con tu estructura de datos
        if (result != 0) {
            printf("Error: %d\n", result);
            return -1;
        }

        // Imprimir la línea (opcional, para depuración)
        //printf("Línea leída: %s\n", line);
    }
    printf("mem: %d\n", heap_free());
	//free(line);
	//fclose(fp);

	irun(it);
	printf("mem: %d\n", heap_free());
	idelinstance(it);
	printf("mem: %d\n", heap_free());
	return 0;
}

