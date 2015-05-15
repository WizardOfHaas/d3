#ifndef TASK_H
#define TASK_H

#include <stddef.h>

#include "mem.h"

enum proc_status{
  SCHEDULE,
  RUNNING,
  YIELDING,
  DONE,
  INIT,
  DEINIT,
  READY
};

typedef struct {
  uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
}registers;

typedef struct ctx_tag{
  mp_t *heap;
  registers regs;
}ctx_t;

typedef struct proc_tag{
  char *name;
  int (*main)(struct proc_t *self);
  int (*init)(struct proc_t *self);
  int (*deinit)(struct proc_t *self);
  int (*yield)(struct proc_t *self);
  int (*schedule)(struct proc_t *self);
  enum proc_status status;
  ctx_t *ctx;

  struct proc_t *prev;
  struct proc_t *next;
}proc_t;

typedef struct task_que_tag{
  size_t tasks;

  proc_t *head;
  proc_t *tail;

  proc_t *current;
}task_que_t;

void init_tasker();

#endif
