#include <stdint.h>
#include "drivers/vga.h"

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

void vga_putentry_at(char c, uint8_t color, int x, int y){
    const int index = y * 80 + x;
    VGA_MEMORY[index] = (uint16_t)c | (uint16_t)color << 8;
}

void vga_clear(uint8_t color){
    for (int i = 0; i < 80 * 25; i++) {
        VGA_MEMORY[i] = (uint16_t)' ' | (uint16_t)color << 8;
    }
}