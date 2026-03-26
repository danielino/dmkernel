#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#define TIMER_HZ 100

void timer_irq_handler();
uint32_t timer_get_ticks(void);
void timer_init();

#endif
