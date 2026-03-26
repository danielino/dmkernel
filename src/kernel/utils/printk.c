#include "kernel/utils/printk.h"

#include "kernel/terminal.h"

#include <stdarg.h>
#include <stdint.h>

static void print_int(int n)
{
	if (n == 0) {
		terminal_putchar('0');
		return;
	}
	if (n < 0) {
		terminal_putchar('-');
		n = -n;
	}

	char buf[12];
	int i = 0;
	while (n > 0) {
		buf[i++] = '0' + (n % 10); // cifra come carattere
		n /= 10;
	}
	while (i > 0)
		terminal_putchar(buf[--i]); // stampa al contrario
}

static void print_hex(uint32_t n)
{
	static const char digits[] = "0123456789abcdef";
	char buf[8];
	int i = 0;

	if (n == 0) {
		terminal_write("0x0");
		return;
	}

	terminal_write("0x");
	while (n > 0) {
		buf[i++] = digits[n % 16]; // prendi il nibble
		n >>= 4;				   // shift di 4 bit = dividi per 16
	}
	while (i > 0)
		terminal_putchar(buf[--i]);
}

void printk(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	for (int i = 0; fmt[i] != '\0'; i++) {
		if (fmt[i] != '%') {
			terminal_putchar(fmt[i]);
			continue;
		}
		i++;
		switch (fmt[i]) {
		case 's':
			terminal_write(va_arg(args, char*));
			break;
		case 'd':
			print_int(va_arg(args, int));
			break;
		case 'x':
			print_hex(va_arg(args, uint32_t));
			break;
		case 'c':
			terminal_putchar(va_arg(args, int));
			break;
		case '%':
			terminal_putchar('%');
			break;
		}
	}

	va_end(args);
}