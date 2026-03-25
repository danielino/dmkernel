#include <stdint.h>
#include "kernel/terminal.h"
#include "kernel/timer.h"
#include "drivers/keyboard.h"
#include "arch/x86/idt.h"
#include "arch/x86/pic.h"
#include "arch/x86/io.h"

/*
 * This function is called from assembly when
 * a divide-by-zero exception occurs.
 */
void isr0_handler() {
    terminal_write("EXCEPTION: DIVIDE BY ZERO\n");

    /*
     * Infinite loop to stop execution.
     * In a real OS, we would handle the error gracefully.
     */
    while (1);
}

void init_irq(){
    pic_init();
    pic_unmask_irq(0); // enable timer at irq0
    pic_unmask_irq(1); // enable keyboard at irq1
}

void kernel_main() {
    terminal_initialize();

    terminal_write("Kernel started\n");

    /*
     * Initialize interrupt system
     */
    idt_init();

    /*
     * initialize pic
     */
    init_irq();
    asm volatile("sti");

    terminal_write("Waiting for command...\n");

    /*
     * This will trigger interrupt 0
     */
    //asm volatile("int $0"); 

    while (1);
}