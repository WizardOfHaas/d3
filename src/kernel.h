#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include "term.h"

typedef enum{
  false,
  true
} bool;

term_t tty0;

void kernel_panic(const char* c);

#endif
