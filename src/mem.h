#include <stddef.h>

#include "multiboot.h"

typedef struct memory_page_tag{
  void *adress;
  size_t size;
  struct mp_t *next;
}mp_t;

mp_t* free;
mp_t* used;
int mem_size;

void* memmove(void* dstptr, const void* srcptr, size_t size);
void init_mm(multiboot_info_t* mbd);
