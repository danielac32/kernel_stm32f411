#include <interrupt.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stm32.h>
#include <usb_cdc_conf.h>
//#include <memory.h>
//#include <kernel.h>
//#include <process.h>
#include <syscall.h>
#include <elf.h>
#include <gpio.h>
#include <xinu.h> 


void *SVC_XINU_NULLPROCESS(uint32 *sp){
    struct procent * p;
    p = (struct procent *) sp[1]; 
            asm volatile ("mov r0, %0" : : "r" (p->prstkptr));
                    asm volatile("ldmia r0!, {r4-r11} ");
                    asm volatile ("msr psp, r0");
                    asm volatile (
                          "ldr r0, =0xFFFFFFFD" "\n\t"
                          "mov lr, r0" "\n\t"
                          "bx lr");
    return sp; 
}
void *SVC_XINU_PUTC (uint32 *sp){
    //while(usbd_ep_write(&udev, CDC_TXD_EP, &sp[2], 1)==-1);
    putc(sp[1],sp[2]);
    return sp;  
}
void *SVC_XINU_PUTS (uint32 *sp){
    //char *str = (char *)sp[1];
    //int size = sp[2];

    //while(usbd_ep_write(&udev, CDC_TXD_EP, str, size)==-1);
    write(sp[1],sp[2],sp[3]);
    return sp;  
}
void *SVC_XINU_GETC(uint32 *sp){
    //sp[0]=fifo[0];
    return sp;  
}
void *SVC_XINU_GETS(uint32 *sp){
     //sp[0]=&fifo[0];
     return sp;  
}
void *SVC_XINU_CLOSE(uint32 *sp){
return sp;  
    
}  /* Devices */
void *SVC_XINU_OPEN (uint32 *sp){
return sp;  
    
}
void *SVC_XINU_READ (uint32 *sp){
return sp;  
    
}
void *SVC_XINU_WRITE (uint32 *sp){
return sp;  
    
}
void *SVC_XINU_CREATE (uint32 *sp){
//sp[0]=create((void *)sp[1], sp[2], 3, (char *)sp[3], 0);
return sp;  
    
} /* Processes */ 
void *SVC_XINU_KILL(uint32 *sp){
    
    return sp;  
} /* Exit */
void *SVC_XINU_READY (uint32 *sp){

     
    return sp;  
}
void *SVC_XINU_SLEEP (uint32 *sp){
    sleepms(sp[1]);
return sp;  
}
void *SVC_XINU_FREE (uint32 *sp){
    free((void *)sp[1]);
    return sp;  
}
void *SVC_XINU_MALLOC(uint32 *sp){
    sp[0] = malloc(sp[1]);
    return sp;  
} 
void *SVC_XINU_GETDEV (uint32 *sp){

    return sp;  
}
void *SVC_XINU_SEMA_INIT(uint32 *sp){

    return sp;  
}  /* Semapho3 */
void *SVC_XINU_SEMA_SIGNAL(uint32 *sp){

    return sp;  
} 
void *SVC_XINU_SEMA_WAIT (uint32 *sp){

    return sp;  
}
void *SVC_XINU_PIN_MODE (uint32 *sp){

    GPIO_TypeDef *p;
    p = (GPIO_TypeDef *)sp[1];
    hw_cfg_pin(p,sp[2],sp[3]);
    return sp;  
}

void *SVC_XINU_PIN_SET (uint32 *sp){

    GPIO_TypeDef *p;
    p = (GPIO_TypeDef *)sp[1];
    hw_set_pin(p, sp[2],sp[3]);
    return sp;  
}
void *SVC_XINU_PIN_GET (uint32 *sp){
    GPIO_TypeDef *p;
    p = (GPIO_TypeDef *)sp[1];
    sp[0] = hw_get_pin(p,sp[2]);
    return sp;  
}
void *SVC_XINU_FREE_HEAP(uint32 *sp){
     sp[0]=heap_free();
     return sp;  
} 
void *SVC_XINU_GETPID (uint32 *sp){

    
    return sp;  
}
void *SVC_XINU_EXIST (uint32 *sp){


    return sp;  
}
void *SVC_XINU_FS_INIT (uint32 *sp){

    return sp;  
}
void *SVC_XINU_ATTACH_MEDIA(uint32 *sp){

    return sp;  
} 
void *SVC_XINU_ATTACH_LOCKS (uint32 *sp){

    return sp;  
} //void *SVC_XINU_LIST_DIR 25
void *SVC_XINU_FOPEN(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FSEEK(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FREAD(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FWRITE(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FCLOSE(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FTELL(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FREWIN(uint32 *sp){

    return sp;  
}
void *SVC_XINU_TRUNCATE(uint32 *sp){

    return sp;  
}
void *SVC_XINU_SIZE(uint32 *sp){
    
    return sp;  
}
void *SVC_XINU_AVAILABLE(uint32 *sp){

    return sp;  
}
void *SVC_XINU_MKDIR(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FEOF(uint32 *sp){

    return sp;  
}
void *SVC_XINU_OPENDIR(uint32 *sp){

    return sp;  
}
void *SVC_XINU_READDIR(uint32 *sp){

    return sp;  
}
void *SVC_XINU_REMOVE(uint32 *sp){

    return sp;  
}
void *SVC_XINU_RENAME(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FORMAT(uint32 *sp){

    return sp;  
}
void *SVC_XINU_MOUNT(uint32 *sp){

    return sp;  
}
void *SVC_XINU_UNMOUNT(uint32 *sp){

    return sp;  
}
void *SVC_XINU_DISK_FREE(uint32 *sp){

    return sp;  
}
void *SVC_XINU_CLOSEDIR(uint32 *sp){

    return sp;  
}
void *SVC_XINU_CREATEDIRECTORY(uint32 *sp){

    return sp;  
}
void *SVC_XINU_IS_DIR(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FFLUSH(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FGETC(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FGETCS(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FPUTC(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FPUTCS(uint32 *sp){

    return sp;  
}
void *SVC_XINU_FGETPOS(uint32 *sp){

    return sp;  
}
void *SVC_XINU_NUM_TASK (uint32 *sp){

 
     return sp;  
}
void *SVC_XINU_STRUCT_TASK(uint32 *sp){
 

return sp;  
} 
void *SVC_XINU_LOAD_ELF(uint32 *sp){
    sp[0]=elf_execve((char *)sp[1],(exec_img *)sp[2]);
    return sp;  
}
void *SVC_XINU_HEAP_FREE(uint32 *sp){

    return sp;  
}
void *SVC_XINU_YIELD(uint32 *sp){
  
return sp;  
}
void *SVC_XINU_GLOBAL_PATH(uint32 *sp){
 
return sp;  
}
void *SVC_XINU_UPDATE_PATH(uint32 *sp){
 
return sp;  
}
void *SVC_XINU_GET_PATH(uint32 *sp){
 
return sp;  
}
void *SVC_XINU_CD(uint32 *sp){
    
    return sp;  
}
void *SVC_XINU_JSON(uint32 *sp){

    return sp;  
}
void *SVC_XINU_GET_LEN(uint32 *sp){

return sp;    
}


