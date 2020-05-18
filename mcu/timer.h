/*
 * Only general-purpose timers here (TIM2 to TIM5)
 */

#ifndef MCU_TIMER_H
#define MCU_TIMER_H

#include <stdint.h>
#include <lib/bits.h>

struct timer {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t __reserved;
	volatile uint32_t CCR[4];
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
};

#define TIM2_BASE	0x40000000U
#define TIM3_BASE	0x40000400U
#define TIM4_BASE	0x40000800U
#define TIM5_BASE	0x40000C00U

#define TIM2		((struct timer *)TIM2_BASE)
#define TIM3		((struct timer *)TIM3_BASE)
#define TIM4		((struct timer *)TIM4_BASE)
#define TIM5		((struct timer *)TIM5_BASE)

/* CR1 register bits */
/* Direction (0: upcounter, 1: downcounter) */
#define TIM_CR1_DIR_BIT		BIT(4)

/* Counter enable */
#define TIM_CR1_CEN_BIT		BIT(0)

#define timer_compare_set(base, channel, value)	\
			base->CCR[channel - 1]= value

#define timer_start(base) \
			base->CR1 |= TIM_CR1_CEN_BIT

#define timer_stop(base) \
			base->CR1 &= ~TIM_CR1_CEN_BIT

void timer_setup_pwm(struct timer *base, int channel);
void timer_setup(struct timer *base, int prescaler, int reload);

#endif

