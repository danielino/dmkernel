#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void vga_putentry_at(char c, uint8_t color, int x, int y);
void vga_clear(uint8_t color);

#endif