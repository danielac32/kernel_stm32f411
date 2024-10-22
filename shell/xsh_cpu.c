/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>

 
#define CPU_CLOCK_HZ 84000000  // 100 MHz
#define TICK_HZ 100000000 / 1000            // 1 ms por tick


uint32 totalTicks = 0;    // Contador de tiempo total (ticks)
extern uint32 idleTicks;     // Contador de tiempo en la tarea idle

 

uint32 calculateIdleTimePercentage(void) {
    totalTicks = (uint32)cycleCount();
    if (totalTicks == 0) {
        return 0;  // Evitar división por cero
    }

    // Calcular el porcentaje de tiempo en idle
    uint32 idlePercentage = (idleTicks / totalTicks) ;
    return idlePercentage;
}
uint32 calculateProcessingTimePercentage(void) {


    totalTicks = (uint32)cycleCount();
    if (totalTicks == 0) {
        return 0.0;  // Evitar división por cero
    }

    // Calcular el porcentaje de tiempo procesando tareas
    uint32_t processingTicks = totalTicks - idleTicks;  // Ticks en los que el CPU está procesando
    uint32 processingPercentage = (processingTicks / totalTicks);

    return processingPercentage;
}

void showIdleTime(void) {
    uint32 idlePercentage = calculateProcessingTimePercentage();//calculateIdleTimePercentage();
    printf(" time: %d\n", idlePercentage);
}

shellcmd xsh_cpu(int nargs, char *args[])
{
	 showIdleTime();
  
 
	return 0;
}
