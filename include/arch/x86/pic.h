#ifndef PIC_H
#define PIC_H

/*
 * PIC I/O ports
 */
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

/*
 * PIC initalization control words
 */
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

/*
 * IRQ base offsets
 */

#define PIC1_OFFSET 0x20 // IRQ0 -> interrupt 32
#define PIC2_OFFSET 0x28 // IRQ8 -> interrupt 40

#define PIC_EOI 0x20
/*
 * initialize pic
 */
void pic_init(void);

/*
 * disable an IRQ
 */
void pic_mask_irq(unsigned int irq);

/*
 * enable an IRQ
 */
void pic_unmask_irq(unsigned int irq);

/*
 * send End Of Interrupt (EOI)
 */
void pic_send_eoi(unsigned int irq);

#endif