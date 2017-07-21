#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include "term.h"

typedef enum{
  false,
  true
} bool;

struct regs{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

term_t tty0;
int uptime;

void kernel_panic(const char* c);

#endif
