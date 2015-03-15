//Screen/terminal stuff

enum vga_color
  {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
  };

typedef struct term_tag{
  size_t width;
  size_t height;
  uint8_t color;

  int xpos;
  int ypos;

  uint16_t* buffer;
}term_t;

void term_init(term_t* term, size_t width, size_t height, enum vga_color fg, enum vga_color bg, uint16_t* buffer);
void term_clear(term_t* term, char c);
void term_setcolor(term_t* term, uint8_t color);
void term_writestring(term_t* term, const char* data);
