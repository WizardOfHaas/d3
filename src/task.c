#include "task.h"
#include "mem.h"
#include "term.h"
#include "kernel.h"

task_que_t task_que;

int test_task_A(proc_t *self){
  term_writestring(&tty0, "A");
  task_yield(self);
}

int test_task_B(proc_t *self){
  term_writestring(&tty0, "B");
  task_yield(self);
}

void init_tasker(){
  task_que.tasks = 0;
  task_que.head = NULL;
  task_que.tail = NULL;

  proc_t testA, testB;
  init_task(&testA, "test_A", test_task_A, NULL, NULL, NULL, NULL);
  init_task(&testB, "test_B", test_task_B, NULL, NULL, NULL, NULL);
  task_schedule(&testA);
  task_schedule(&testB);
}

//Fill out proc_t and setup needed data/heap/stuff
void init_task(proc_t *proc, char *name,
	       int (*main)(struct proc_t *self),
	       int (*init)(struct proc_t *self),
	       int (*deinit)(struct proc_t *self),
	       int (*schedule)(struct proc_t *self),
	       int (*yield)(struct proc_t *self)){
  //Fill in supplied data...
  proc->name = name;
  proc->main = main;
  proc->init = init;
  proc->deinit = deinit;
  proc->schedule = schedule;
  proc->yield = yield;

  //Allocate proccess heap and other context stuff...
  proc->ctx = (ctx_t*)malloc(sizeof(ctx_t))->address;
  proc->ctx->heap = malloc(4096);

  proc->ctx->regs.eax = 0;
  proc->ctx->regs.ebx = 0;
  proc->ctx->regs.ecx = 0;
  proc->ctx->regs.edx = 0;
  proc->ctx->regs.esi = 0;
  proc->ctx->regs.edi = 0;
  proc->ctx->regs.esp = proc->ctx->heap->address + 4096;
  proc->ctx->regs.ebp = 0;
  proc->ctx->regs.eip = (uint32_t)main;

  task_init(proc);
}

void task_schedule(proc_t *proc){
  proc->status = SCHEDULE;
  if(proc->schedule != NULL){
    proc->schedule(proc);
  }

  //Add to task_que
  if(task_que.head == NULL && task_que.tail == NULL){
    task_que.head = proc;
    task_que.tail = proc;
    proc->prev = proc;
    proc->next = proc;
  }else{
    proc->next = task_que.head;
    proc->prev = task_que.tail;
    task_que.tail->next = proc;
    task_que.tail = proc;
  }
  
  task_que.tasks++;
}

void task_run(proc_t *proc){
  proc->status = RUNNING;
  task_que.current = proc;

  if(proc->main != NULL){
    proc->main(proc);
  }
}

void task_init(proc_t *proc){
  proc->status = INIT;

  if(proc->init != NULL){
    proc->init(proc);
  }

  proc->status = READY;
}

void task_deinit(proc_t *proc){
  proc->status = DEINIT;

  if(proc->deinit != NULL){
    proc->deinit(proc);
  }

  free(proc->ctx->heap);
  free(proc->ctx);

  proc->status = DONE;
}

void task_yield(proc_t *proc){
  proc->status = YIELDING;

  if(proc->yield != NULL){
    proc->yield(proc);
  }

  task_run(proc->next);
  proc->status = READY;
}
