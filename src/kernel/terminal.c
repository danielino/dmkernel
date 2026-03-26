#include "kernel/terminal.h"

#include "arch/x86/io.h"
#include "drivers/vga.h"
#include "kernel/utils/string.h"

#include <stddef.h>
#include <stdint.h>

static size_t row;
static size_t col;
static uint8_t color;

void terminal_initialize()
{
	row = 0;
	col = 0;
	color = 0x0F;
	vga_clear(color);
}

static void terminal_scroll()
{
	if (row < 25)
		return;
	uint16_t* buf = (uint16_t*)0xB8000;
	// sposta tutto su di una riga
	memcpy(buf, buf + 80, 80 * 24 * 2);
	// pulisci ultima riga
	for (int c = 0; c < 80; c++)
		vga_putentry_at(' ', color, c, 24);
	row = 24;
}

void terminal_update_cursor(void)
{
	uint16_t pos = row * 80 + col;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		col = 0;
		row++;
		terminal_scroll();
	} else if (c == '\b') {
		if (col > 0) {
			col--;
			vga_putentry_at(' ', color, col, row);
		}
	} else {
		vga_putentry_at(c, color, col, row);
		col++;
		if (col >= 80) {
			col = 0;
			row++;
			terminal_scroll();
		}
	}
	terminal_update_cursor();
}

void terminal_write(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i++) {
		terminal_putchar(str[i]);
	}
}

void terminal_clear(void)
{
	for (int r = 0; r < 25; r++)
		for (int c = 0; c < 80; c++)
			vga_putentry_at(' ', color, c, r);
	row = 0;
	col = 0;
	terminal_update_cursor();
}