#include <stdint.h>

#include "kernel.h"
#include "keybd.h"
#include "io.h"
#include "idt.h"
#include "mem.h"

unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char kbdus_shifted[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
  '(', ')', '_', '+', '\b', /* Backspace */
  '\t',     /* Tab */
  'Q', 'W', 'E', 'R', /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
    0,      /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
 '"', '~',   0,    /* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',      /* 49 */
  'M', '<', '>', '?',   0,        /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

//Data struct for keyboard buffer
int kb_i = 0;
char kb_buffer[256];

//Status of shiftiness
int kb_shifted = 0;

void init_keybd(){
  irq_install_handler(1, keyboard_handler);
}

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r){
  unsigned char scancode;
  /* Read from the keyboard's data buffer */
  scancode = inb(0x60);
  char c;

  if(kbdus[scancode] != 0 && scancode < 88){ //Normal printable character
    if(kb_shifted){
      c = kbdus_shifted[scancode];
    }else{
      c = kbdus[scancode];
    }

    term_putchar(&tty0, c);

    if(scancode == 0x1C){
      term_writestring(&tty0, kb_buffer);

    }

    //Add to keyboard buffer
    kb_buffer[kb_i] = c;
    kb_i++;
  }else if(scancode == 0x2A || scancode == 0x36){ //Shift pressed down
    kb_shifted = 1;
  }else if(scancode == 0xAA || scancode == 0xB6){ //Shift released
    kb_shifted = 0;
  }else if(scancode == 0x3A){ //Caps lock
    kb_shifted = !kb_shifted;
  }
}

//Clear buffer
void clear_kb_buffer(){
  mem_set(&kb_buffer, 0, 256);
  kb_i = 0;
}

//Return the current buffer
char get_kbd_buffer(){
  return kb_buffer;
}

//Put buffer contents into given dest
//...then clear out the buffer
void get_kbd(char *dest){
  while(kb_buffer[kb_i - 1] != "\n"){} //Wait for enter to be pressed...

  mem_cpy(dest, &kb_buffer, kb_i - 1); //Coppy over buffer
  dest[kb_i] = 0;
  clear_kb_buffer(); //Clear out the internal buffer
}