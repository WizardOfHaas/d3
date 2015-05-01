#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "term.h"
#include "mem.h"
#include "str.h"

term_t tty0;

void kernel_panic(const char* c){
  term_setcolor(&tty0, make_color(COLOR_LIGHT_GREY, COLOR_RED));

  term_writestring(&tty0, "\n**KERNEL PANIC**:\n     ");
  term_writestring(&tty0, c);
  
  while(1){}
}

void cmain(multiboot_info_t* mbd)
{
  //Initialize first terminal
  term_init(&tty0, 80, 25, COLOR_LIGHT_GREY, COLOR_BLACK, (uint16_t*) 0xB8000);
 
  //Splash...
  term_writestring(&tty0, "d3 Booting...\n\n");

  //Init memory manager and print some stats...
  term_writestring(&tty0, "Initializing memory manager...");
  init_mm(mbd);
  term_writestring(&tty0, "[OK]\n");

  //List all my free buddies....for debugging
  mp_t *temp = &mm_free;
  
  while(temp->next != NULL){
    term_writestring(&tty0, "\nBuddy:@");
    term_writestring(&tty0, itoa(temp, 16));
    term_writestring(&tty0, "\n |Size:");
    term_writestring(&tty0, itoa(temp->size, 10));
    term_writestring(&tty0, "\n |Pointies:");
    term_writestring(&tty0, itoa(temp->prev, 16));
    term_writestring(&tty0, "<->");
    term_writestring(&tty0, itoa(temp->next, 16));
    temp = temp->next;
  }

  term_writestring(&tty0, "\nCalling Malloc:\n");
  mp_t *test = malloc(1024);
  term_writestring(&tty0, itoa(test->size, 16));
  term_writestring(&tty0, "b\n Page:@");
  term_writestring(&tty0, itoa(test->address, 16));
}
