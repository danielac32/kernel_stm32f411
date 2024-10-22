
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <syscall.h>
#include <process.h>
 

syscall_t syscallp;

struct t_state
{
	char	state[16];
};

const struct t_state state_t[]={
	{"free"},
	{"curr"},
	{"ready"},
	{"recv "},
	{"sleep"},
	{"susp "},
	{"wait "}

};

int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
    
    int32   i;          /* walks through args array */

        /* check all NPROC slots */
    ///char *state[]={"free ", "curr ", "ready", "recv ", "sleep", "susp ","wait "};

    /*printf("\ntable of current processes\n");
    printf("\nname\tid\tstate\tstack base\tstack size\n--\n");
    for (i = 0; i < NTHREAD; i++) {
    	task_t *prptr=(task_t *)syscallp.self(i);	

        if (prptr->state == PR_FREE){
            continue;
        }
        printf("%s\t%d", prptr->name, i);
        printf("\t%s",state_t[(int)prptr->state].state);
        printf("\t0x%08x ", prptr->prstkbase);
        printf("\t%d ", prptr->prstklen);
        printf("\n");
    }*/


    printf("%3s %-16s %5s %10s %10s %10s\n",
           "Pid", "Name", "State", "Stack Base",
           "Stack Ptr", "Stack Size");


    for (int i = 0; i < NTHREAD/*MAX_NUMBER_OF_TASKS*/; i++) {
        task_t *prptr = (task_t *)syscallp.self(i);//&task[i];
        if (prptr->state == PR_FREE) {  
            continue;
        }
        printf("%3d %-16s %5s 0x%08X 0x%08X %8d\n",
            i, prptr->name, state_t[(int)prptr->state].state,
            prptr->prstkbase,(uint32)prptr->sp, prptr->prstklen);
    }

    
	return 0;
}