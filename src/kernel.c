#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "term.h"
#include "mem.h"
#include "str.h"
#include "task.h"
#include "vm.h"
#include "vm_ops.h"
#include "gdt.h"
#include "idt.h"

#include "io.h"
#include "fat12.h"

#define from_bcd(val)  ((val / 16) * 10 + (val & 0xf))

term_t tty0;

void kernel_panic(const char* c){
  term_setcolor(&tty0, make_color(COLOR_LIGHT_GREY, COLOR_RED));

  term_writestring(&tty0, "\n**KERNEL PANIC**:\n     ");
  term_writestring(&tty0, c);

  while(1){}
}

void sleep(t){ //Actually implement later
  int time = get_time();
  while(time < t){
    time = get_time();
  }
}

void cmain(multiboot_info_t* mbd)
{
  //Initialize first terminal
  term_init(&tty0, 80, 25, COLOR_LIGHT_GREY, COLOR_BLACK, (uint16_t*) 0xB8000);

  //Splash...
  term_writestring(&tty0, "d3 Booting...\n\n");

  //Init GDT...
  term_writestring(&tty0, "Initializing GDT...");
  gdt_init();
  term_writestring(&tty0, "[OK]\n");

  //Init IDT...
  term_writestring(&tty0, "Initializing IDT/ISRs...");
  init_idt();
  term_writestring(&tty0, "[OK]\n");

  //Init memory manager and print some stats...
  term_writestring(&tty0, "Initializing memory manager...");
  init_mm(mbd);
  term_writestring(&tty0, "[OK]\n");

  //Init task manager...
  term_writestring(&tty0, "Initializing task manager...");
  init_tasker();
  term_writestring(&tty0, "[OK]\n");

  //Init vm manager...
  term_writestring(&tty0, "Initializing vm manager...");
  init_vmm();
  term_writestring(&tty0, "[OK]\n");

  //Init fat12 driver
  term_writestring(&tty0, "Initializing fat12 driver...");
  //init_fat12();
  term_writestring(&tty0, "[OK]\n");

  term_writestring(&tty0, ">");
  char c;
  while(c = getchar()){
    term_putchar(&tty0, c);
  }
  term_writestring(&tty0, ">");
}

void cmos_dump(uint16_t * values){
  uint16_t index;
  for (index = 0; index < 128; ++index) {
    outb(0x70, index);
    values[index] = inb(0x71);
  }
}

int get_time(){
  uint16_t values[128]; /* CMOS dump */
  cmos_dump(values);

  uint16_t hours;
  uint16_t minutes;
  uint16_t seconds;
  uint16_t day;
  uint16_t month;

  hours   = from_bcd(values[4]);
  minutes = from_bcd(values[2]);
  seconds = from_bcd(values[0]);
  month = from_bcd(values[8]);
  day   = from_bcd(values[7]);

  return seconds + minutes * 60 + hours * 3600;
}
