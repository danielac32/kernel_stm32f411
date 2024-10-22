

#ifndef QUEUE_H
#define QUEUE_H
#include "process.h"
#include "kernel.h"

 

#define STACK_LENGTH 20
#define EMPTY (-1)
#define INT_MIN (-2147483648)
#define STACK_EMPTY INT_MIN

extern int stack[];
extern int top;
bool push(int value);
int pop();
bool is_empty();

#if 1
#define ERR_OK      0
#define ERR_NORES   1
#define ERR_PARAMS    2

typedef struct {
  task_t *first;
  task_t *last;
} queue_t;

typedef struct {
  uint32 value;
  queue_t waitq;
  
}sem_t;

uint32 sem_init(sem_t * sem, uint32 value);
uint32 sem_signal(sem_t * sem);
uint32 sem_signaln(sem_t *sem,int);
uint32 sem_wait(sem_t * sem);
uint32 sem_count(sem_t * sem);

void queue_init(queue_t *q);

void queue_push(queue_t *q, task_t *t);

void queue_pushsort(queue_t *q, task_t *t);

task_t* queue_pop(queue_t *q);
task_t* queue_peek(queue_t *q);


extern queue_t ready_queue;
extern queue_t sleep_queue;

#endif



#endif