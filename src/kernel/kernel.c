#include <stdint.h>
#include "multiboot.h"
#include "kernel/terminal.h"
#include "kernel/timer.h"
#include "kernel/shell.h"
#include "kernel/utils/printk.h"
#include "drivers/keyboard.h"
#include "arch/x86/idt.h"
#include "arch/x86/pic.h"
#include "arch/x86/io.h"
#include "arch/x86/irq.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "mm/heap.h"

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
    irq_init();
    irq_register(0, timer_irq_handler);
    irq_register(1, keyboard_irq_handler);
    pic_unmask_irq(0); // enable timer at irq0
    pic_unmask_irq(1); // enable keyboard at irq1
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
    // it must be the first function call of the function scope
    terminal_initialize();

    if (magic != MULTIBOOT_MAGIC) {
        printk(KERN_ERR "Invalid multiboot magic: %x\n", magic);
        while (1);
    }
    printk(KERN_INFO "Memory lower: %d KB\n", mbi->mem_lower);
    printk(KERN_INFO "Memory upper: %d KB\n", mbi->mem_upper);

    // initialize memory management
    pmm_init(mbi);
    vmm_init();
    heap_init();


    printk(KERN_INFO "Kernel started\n");

    /*
     * Initialize interrupt system
     */
    idt_init();

    timer_init();
    /*
     * initialize pic
     */
    init_irq();
    asm volatile("sti");

    printk(KERN_INFO "Waiting for command...\n");

    /*
     * This will trigger interrupt 0
     */
    //asm volatile("int $0"); 

    shell_run();
}