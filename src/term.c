#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "term.h"
#include "mem.h"
#include "io.h"

uint8_t make_color(enum vga_color fg, enum vga_color bg){
  return fg | bg << 4;
}
 
uint16_t make_vgaentry(term_t* term, char c){
  uint16_t c16 = c;
  uint16_t color16 = term->color;
  return c16 | color16 << 8;
}

size_t strlen(const char* str){
  size_t ret = 0;
  while ( str[ret] != 0 )
    ret++;
  return ret;
}

void term_init(term_t* term, size_t width, size_t height, enum vga_color fg, enum vga_color bg, uint16_t* buffer){
  term->width = width;
  term->height = height;
  term->color = make_color(fg, bg);
  term->buffer = buffer;
  term->xpos = 0;
  term->ypos = 0;
 
  term_clear(term, ' ');
}

void term_clear(term_t* term, char c){
  for ( size_t y = 0; y < term->height; y++ )
    {
      for ( size_t x = 0; x < term->width; x++ )
	{
	  const size_t index = y * term->width + x;
	  term->buffer[index] = make_vgaentry(term, c);
	}
    }
}

void term_scroll(term_t* term){
  mem_cpy(term->buffer, term->buffer + term->width, term->width * term->height * 2);
}

/*void term_scroll(term_t* term){
  for(int i = 0; i < term->height; i++){
    for(int m = 0; m < term->width; m++){
      term->buffer[i * term->width + m] = term->buffer[(i + 1) * term->width + m];
    }
  }
}*/
 
void term_setcolor(term_t* term, uint8_t color){
  term->color = color;
}
 
void term_putentryat(term_t* term, char c, size_t x, size_t y){
  const size_t index = y * term->width + x;
  term->buffer[index] = make_vgaentry(term, c);
}
 
void term_putchar(term_t* term, char c){
  if(c == '\n'){
    term->xpos = 0;
    term->ypos++;
  }else if(c == '\b'){
    if(term->xpos > 0){
      term->xpos--;
      term_putentryat(term, ' ', term->xpos, term->ypos);
    }
  }

  if(term->xpos + 1 >= term->width){ //Do I need to wrap?
    term->xpos = 0; //Move x to 0
	   term->ypos++; //Go to next line
  }

  if(term->ypos >= term->height){ //Do I need to scroll?
    term_scroll(term); //Scroll 1 line
    term->ypos--; //Push back y pos
  }

  if(c != '\n' && c != '\b'){
    term_putentryat(term, c, term->xpos, term->ypos); //Put the character on screen

    if(term->xpos < term->width){ //Advance to (now) sane screen location
      term->xpos++;
    }
  }

  update_cursor(term->ypos, term->xpos);
}
 
void term_writestring(term_t* term, const char* data){
  size_t datalen = strlen(data);
  for (size_t i = 0; i < datalen; i++){
    term_putchar(term, data[i]);
  }
}

void update_cursor(int row, int col){
    unsigned short position= (row * 80) + col;
 
    // cursor LOW port to vga INDEX register
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}