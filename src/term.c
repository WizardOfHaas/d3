#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "term.h"
#include "mem.h"

uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
  return fg | bg << 4;
}
 
uint16_t make_vgaentry(term_t* term, char c)
{
  uint16_t c16 = c;
  uint16_t color16 = term->color;
  return c16 | color16 << 8;
}

size_t strlen(const char* str)
{
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
  //HERE IS PROBLEM!
  movemem(term->buffer - 80, term->buffer, 80*25);
}
 
void term_setcolor(term_t* term, uint8_t color)
{
  term->color = color;
  term->xpos--;
}
 
void term_putentryat(term_t* term, char c, size_t x, size_t y)
{
  const size_t index = y * term->width + x;
  term->buffer[index] = make_vgaentry(term, c);
}
 
void term_putchar(term_t* term, char c)
{
  if(c == '\n'){
    term->ypos = 0;
    term->xpos++;
  }else{
    term_putentryat(term, c, term->ypos, term->xpos);

    if ( ++term->ypos == term->width )
      {
	term->ypos = 0;
	if ( ++term->xpos == term->height )
	  {
	    term_scroll(term);
	  }
      }
  }
}
 
void term_writestring(term_t* term, const char* data)
{
  size_t datalen = strlen(data);
  for ( size_t i = 0; i < datalen; i++ ){
    term_putchar(term, data[i]);
  }
}
