#include "arch/x86/pic.h"

#include "arch/x86/io.h"

void pic_init(void)
{

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC1_DATA, PIC1_OFFSET); // (0x20)
	outb(PIC1_DATA, 0x04);		  // ICW3: slave on IRQ2
	outb(PIC1_DATA, ICW4_8086);

	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); // same as pic1 (0x11)
	outb(PIC2_DATA, PIC2_OFFSET);			   // (0x28)
	outb(PIC2_DATA, 0x02);					   // ICW3: I am on IRQ2
	outb(PIC2_DATA, ICW4_8086);

	// mask irq
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
}

void pic_send_eoi(unsigned int irq)
{
	/* if irq >= 8, send command also to slave and then send to master*/
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

void pic_unmask_irq(unsigned int irq)
{
	uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
	uint8_t mask = inb(port);
	outb(port, mask & ~(1 << (irq % 8)));
}

void pic_mask_irq(unsigned int irq)
{
	uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
	uint8_t mask = inb(port);
	outb(port, mask | (1 << (irq % 8)));
}