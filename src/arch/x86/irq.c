#include "arch/x86/irq.h"
#include "arch/x86/pic.h"

static irq_handler_t irq_handlers[16];

void irq_init(void){
    for(int i = 0; i < 16 ; i++)
        irq_handlers[i] = 0;
}

int irq_register(uint8_t irq, irq_handler_t handler){
    if(irq >= 16 || irq_handlers[irq] != 0)
        return -1;
    irq_handlers[irq] = handler;
    return 0;
}

int irq_unregister(uint8_t irq){
    if(irq >= 16 || irq_handlers[irq] == 0)
        return -1;
    irq_handlers[irq] = 0;
    return 0;
}

void irq_dispatch(uint32_t irq){
    if(irq < 16 && irq_handlers[irq])
        irq_handlers[irq]();
    pic_send_eoi(irq);
}