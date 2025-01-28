/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>

 
void update_runtime() {
    static uint32 last_time = 0;
    uint32 current_time = cycleCount(); // Obtener el tiempo actual del sistema

    if (last_time != 0) {
        uint32 elapsed_time = current_time - last_time;

        // Actualizar el tiempo de ejecución de la tarea actual
       // if (currpid != NULLPROC) {
            proctab[currpid].runtime += elapsed_time;
        //}
    }

    last_time = current_time;
    printf("last time: %d\n", last_time);
}


void calculate_cpu_usage() {
    uint32 total_runtime = 0;

    // Calcular el tiempo total de ejecución de todas las tareas
    uint32 q=disable();
    for (int i = 0; i < NPROC; i++) {
        if (proctab[i].prstate != PR_FREE) {
            total_runtime += proctab[i].runtime;
        }
    }
    restore(q);
   // printf("total runtime: %d\n", total_runtime);

    if (total_runtime == 0) {
        printf("Error: Tiempo total de ejecución es 0.\n");
        return;
    }

    q=disable();
    // Calcular el uso de CPU para cada tarea
    for (int i = 0; i < NPROC; i++) {
        if (proctab[i].prstate != PR_FREE) {
            uint32 cpu_usage = proctab[i].runtime / total_runtime * 100;
            kprintf("Tarea: %s, Uso de CPU: %d\n", proctab[i].prname, (uint32)cpu_usage);
        }
    }
    restore(q);
    // Calcular el uso de CPU total
    uint32 total_cpu_usage = total_runtime / cycleCount() * 100;
    printf("Uso de CPU total: %d\n", (uint32)total_cpu_usage);
   
}
 



shellcmd xsh_cpu(int nargs, char *args[])
{
	  update_runtime();       // Actualizar el tiempo de ejecución
      calculate_cpu_usage();  // Calcular y mostrar el uso de CPU
 
	return 0;
}
