#include <stdint.h>

#include "arch/x86/pic.h"
#include "arch/x86/io.h"

static uint32_t tick = 0;

void timer_init(){
    outb(0x43, 0x36);
    outb(0x40, 11932 & 0xFF);
    outb(0x40, (11932 >> 8) & 0xFF);
}

void timer_irq_handler() {
    tick++;
    pic_send_eoi(0);
}

uint32_t timer_get_ticks(void) {
    return tick;
}