/* Cortex-M0 */
#include <stdint.h>
#include <mcu/cortex.h>

extern uint32_t __stack__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;

extern void main(void);

struct nvic {
	volatile uint32_t ISER;
	uint32_t __reserved_0[31];
	volatile uint32_t ICER;
	uint32_t __reserved_1[31];
	volatile uint32_t ISPR;
	uint32_t __reserved_2[31];
	volatile uint32_t ICPR;
	uint32_t __reserved_3[31];
	uint32_t __reserved_4[64];
	volatile uint32_t IP[8];
};

struct systick {
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile const uint32_t CALIB;
};

#define SCS_BASE	(0xE000E000UL)
#define SYSTICK_BASE	(SCS_BASE +  0x0010UL)
#define NVIC_BASE	(SCS_BASE +  0x0100UL)

#define NVIC		((struct nvic *)NVIC_BASE)
#define SYSTICK		((struct systick *)SYSTICK_BASE)

/* SysTick bits */
#define SYSTICK_CTRL_ENABLE_BIT		(1 << 0)
#define SYSTICK_CTRL_TICKINT_BIT	(1 << 1)
#define SYSTICK_CTRL_CLKSOURCE_BIT	(1 << 2)

void nvic_enable_irq(enum nvic_irq irq)
{
	NVIC->ISER = (1UL << (((uint32_t)(int32_t)irq) & 0x1FUL));
}

void nvic_disable_irq(enum nvic_irq irq)
{
	NVIC->ICER = (1UL << (((uint32_t)(int32_t)irq) & 0x1FUL));
}

void systick_init(uint32_t ticks)
{
	SYSTICK->LOAD = ticks - 1;
	SYSTICK->VAL  = 0UL;
	SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE_BIT
			| SYSTICK_CTRL_TICKINT_BIT
			| SYSTICK_CTRL_ENABLE_BIT;
}

/* default dummy handlers */
#define __weak __attribute__((weak))

__weak void nmi_handler(void)
{
	while(1);
}

__weak void hardfault_handler(void)
{
	while(1);
}

__weak void svc_handler(void)
{
	while(1);
}

__weak void pend_sv_handler(void)
{
	while(1);
}

__weak void systick_handler(void)
{
	while(1);
}

void reset_handler(void)
{
	uint32_t *src = &__data_load__;
	uint32_t *dest = &__data_start__;

	while (dest < &__data_end__)
		*(dest++) = *(src++);

	for (uint32_t *bss = &__bss_start__; bss < &__bss_end__; bss++) {
		*bss = 0;
	}

	main();
}

uint32_t __attribute__((section(".isr_vectors"))) isr_table[] = {
	(uint32_t)&__stack__,
	(uint32_t)reset_handler,
	(uint32_t)nmi_handler,
	(uint32_t)hardfault_handler,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)svc_handler,
	(uint32_t)0,
	(uint32_t)0,
	(uint32_t)pend_sv_handler,
	(uint32_t)systick_handler
};

