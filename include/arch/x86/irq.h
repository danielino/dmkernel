#include <stdint.h>

typedef void (*irq_handler_t)(void);

void irq_init(void);
int irq_register(uint8_t irq, irq_handler_t handler);
int irq_unregister(uint8_t irq);
// use int32 as assembly push irq as 32bit number (x86 registry are 32 bits)
void irq_dispatch(uint32_t irq);