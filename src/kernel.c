#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "term.h"
#include "mem.h"

term_t tty0;

void cmain()
{
  term_init(&tty0, 80, 25, COLOR_GREEN, COLOR_BLACK, (uint16_t*) 0xB8000);
 
  term_writestring(&tty0, "Hello, kernel World!\n");
  term_writestring(&tty0, "Welcome to d3\n");
}
