#ifndef KEYBD_H
#define KEYBD_H

void keyboard_handler(struct regs *r);
void init_keybd();

char kb_buffer[256];
int kb_i;

#endif