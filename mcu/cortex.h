#ifndef CORTEX_H
#define CORTEX_H

#define __ENABLE_IRQ \
	do { \
		__asm volatile ("cpsie i" : : : "memory"); \
	} while (0);

#define __DISABLE_IRQ \
	do { \
		__asm volatile ("cpsid i" : : : "memory"); \
	} while (0);

enum nvic_irq {
	NVIC_IRQ_SYSTICK = -1,
};

void nvic_enable_irq(enum nvic_irq irq);
void nvic_disable_irq(enum nvic_irq irq);
void systick_init(uint32_t ticks);

#endif

