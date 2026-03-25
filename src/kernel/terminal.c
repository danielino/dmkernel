#include <stddef.h>
#include <stdint.h>
#include "kernel/terminal.h"
#include "drivers/vga.h"

static size_t row;
static size_t col;
static uint8_t color;

void terminal_initialize(){
    row = 0;
    col = 0;
    color = 0x0F;
    vga_clear(color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        col = 0;
        row++;
        return;
    }

    vga_putentry_at(c, color, col, row);
    col++;

    if (col >= 80) {
        col = 0;
        row++;
    }
}

void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}