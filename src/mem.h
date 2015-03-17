#include <stddef.h>

#include "multiboot.h"

typedef struct memory_page_tag{
  void *adress;
  size_t size;
  struct mp_t *next;
}mp_t;

typedef struct stack_tag{
  void *data[32];
  size_t top;
}stack_t;

stack_t free;
stack_t used;
int mem_size;

void* memmove(void* dstptr, const void* srcptr, size_t size);
void init_mm(multiboot_info_t* mbd);

void init_stack(stack_t* stack);
void stack_push(stack_t* stack, void* el);
void* stack_pop(stack_t* stack);
