#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);
void keyboard_irq_handler(void);
int keyboard_line_ready(void);
void keyboard_readline(char* out, int max);

#endif