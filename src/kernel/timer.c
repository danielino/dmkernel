#include <stdint.h>

#include "arch/x86/pic.h"

static uint32_t tick = 0;

void irq0_handler() {
    tick++;
    pic_send_eoi(0);
}