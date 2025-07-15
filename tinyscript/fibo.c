#include <xinu.h>
#include <gpio.h>
#include "tinyscript.h"
#include "tinyscript_lib.h"
#include <fat_filelib.h>

 
#define ARENA_SIZE 4096

int inchar() {
    return -1;
}
void outchar(int c) {
    putchar(c);
}

static Val getcnt_fn()
{
    return (uint32_t)cycleCount();
}
static Val waitcnt_fn(Val when)
{
    sleepms(when);
    return when;
}
static Val pinout_fn(Val pin, Val onoff)
{
    hw_set_pin(GPIOx(GPIO_C),pin,onoff);
    return onoff;
}
static Val pinin_fn(Val pin)
{
return hw_get_pin(GPIOx(GPIO_A),pin);
}

struct def {
    const char *name;
    intptr_t val;
    int nargs;
} fdefs[] = {
    { "getcnt",    (intptr_t)getcnt_fn, 0 },
    { "waitcnt",   (intptr_t)waitcnt_fn, 1 },
    { "pinout",    (intptr_t)pinout_fn,  2 },
    { "pinin",     (intptr_t)pinin_fn, 1 },
    { "delay",     (intptr_t)waitcnt_fn,1 },
    { NULL, 0 }
};

//char memarena[ARENA_SIZE];

/*
const char *tinyblink=
"func blink(){\n"
"var i=0\n"
"while 1>0 {     \n"
"    pinout(13,1)\n"
"    delay(500)\n"
"    pinout(13,0)\n"
"    delay(500)\n"
"    print \"blink(\",i,\")\"\n"
"  i=i+1\n"
"}\n"
"}\n"
"\n"
"blink()\n";

const char *code = 
"# simple fibonacci demo\n"
"#\n"
"# calculate fibo(n)\n"
"\n"
"func fibo(n) {\n"
"if (n<2){\n"
"return n\n"
"}else{\n"
"return fibo(n-1)+fibo(n-2)\n"
"}\n"
"}\n"
"\n"
"# convert cycles to milliseconds\n"
"func calcms() {\n"
"  ms=(cycles+40000)/80000\n"
"}\n"
"\n"
"var i=1\n"
"var cycles=0\n"
"var ms=0\n"
"var r=0\n"
"while i<=20 {\n"
"  cycles=getcnt()\n"
"  r=fibo(i)\n"
"  cycles=getcnt()-cycles\n"
"  calcms()\n"
"  print \"fibo(\",i,\") = \",r, \" \", ms, \" ms (\", cycles, \" cycles)\"\n"
"  i=i+1\n"
"}\n";
*/

void runscript(const char *filename)
{
    char *tmp=full_path(filename);
    if (tmp==NULL)return;


    FILE *fd = fopen(tmp, "r");
    int r;

    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    char *script = malloc(fileLength);

    r=fread(script, fileLength,1,fd);
    fclose(fd);

    if (r <= 0) {
        printf( "File read error on %s\n", tmp);
        free(script);
        return;
    }
    script[r] = 0;
  //  printf("code: %s\n",script );
    r = TinyScript_Run(script, 0, 1);
    if (r != 0) {
        printf("script error %d\n", r);
    }
    free(script);
}


int tinyscript(int argc, char *argv[])
{    
 
    int err;
    int i;


    printf("init...\n");
    char *buffer=malloc(ARENA_SIZE);
    err = TinyScript_Init(buffer, ARENA_SIZE);
    for (i = 0; fdefs[i].name; i++) {
        err |= TinyScript_Define(fdefs[i].name, CFUNC(fdefs[i].nargs), fdefs[i].val);
    }
    err |= ts_define_funcs();
    if (err != 0) {
        printf("Initialization of interpreter failed!\n");
        free(buffer);
        return 1;
    }
   
    runscript(argv[argc-1]);
    free(buffer);

    return 0;
}

