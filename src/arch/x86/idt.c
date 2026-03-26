#include "arch/x86/idt.h"

#include <stdint.h>

/*
 * The x86 IDT can hold up to 256 interrupt descriptors.
 */
#define IDT_ENTRIES 256

/*
 * One entry in the IDT.
 *
 * This structure defines how the CPU should jump
 * to an interrupt handler.
 */
struct idt_entry {
	uint16_t offset_low;  // Lower 16 bits of handler function address
	uint16_t selector;	  // Code segment selector in GDT
	uint8_t zero;		  // Must be zero
	uint8_t type_attr;	  // Flags (type, privilege level, present bit)
	uint16_t offset_high; // Upper 16 bits of handler function address
} __attribute__((packed));

/*
 * Pointer structure used by the lidt instruction.
 */
struct idt_ptr {
	uint16_t limit; // Size of the IDT - 1
	uint32_t base;	// Address of the first IDT entry
} __attribute__((packed));

/*
 * These functions are implemented in assembly.
 */
extern void idt_load(uint32_t);
extern void isr0();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

static void (*irq_stubs[16])() = {irq0, irq1, irq2,	 irq3,	irq4,  irq5,  irq6,	 irq7,
								  irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15};

/*
 * The actual IDT (array of entries)
 */
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

/*
 * Set one entry in the IDT.
 *
 * n       = interrupt number (0–255)
 * handler = address of the function to call
 */
static void idt_set_gate(int n, uint32_t handler)
{
	idt[n].offset_low = handler & 0xFFFF;
	idt[n].selector = 0x10; // Kernel code segment (from GDT)
	idt[n].zero = 0;

	/*
	 * 0x8E = 10001110b
	 *  - Present bit = 1
	 *  - Descriptor privilege level = 00 (kernel)
	 *  - Type = 1110 (32-bit interrupt gate)
	 */
	idt[n].type_attr = 0x8E;

	idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

/*
 * Initialize the IDT
 */
void idt_init()
{
	/*
	 * Set IDT pointer values
	 */
	idtp.limit = sizeof(idt) - 1;
	idtp.base = (uint32_t)&idt;

	/*
	 * Clear all entries (set them to null handlers)
	 */
	for (int i = 0; i < IDT_ENTRIES; i++) {
		idt_set_gate(i, 0);
	}

	/*
	 * Register ISR 0 (Divide by zero exception)
	 */
	idt_set_gate(0, (uint32_t)isr0);
	for (int i = 0; i < 16; i++) {
		idt_set_gate(32 + i, (uint32_t)irq_stubs[i]);
	}

	/*
	 * Load IDT into CPU using lidt instruction
	 */
	idt_load((uint32_t)&idtp);
}