#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/*
 * Initializes the Interrupt Descriptor Table (IDT)
 * and loads it into the CPU.
 *
 * After this call, the CPU knows how to handle interrupts.
 */
void idt_init();

#endif