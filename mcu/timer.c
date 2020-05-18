#include <mcu/timer.h>

/*
 * Basic timers have 4 compare channels
 *
 * Channels are numbered from 1!
 */
void timer_setup_pwm(struct timer *base, int channel)
{
	base->CCER &= ~(0b0011 << (4 * (channel - 1)));
	base->CCER |=  (0b0001 << (4 * (channel - 1))); /* Output enable */

	/* Set OCM bits to 0b110: PWM mode 1 in CCMRx register */
	if (channel > 2) {
		/* Channels 3, 4: CCMR2 */
		base->CCMR2 &= ~(0xFF << (8 * (channel - 1 - 2)));
		base->CCMR2 |=  ((0b110 << 4) << (8 * (channel - 1)));
	} else {
		/* Channels 1, 2: CCMR1 */
		base->CCMR1 &= ~(0xFF << (8 * (channel - 1)));
		base->CCMR1 |=  ((0b110 << 4) << (8 * (channel - 1)));
	}
}

void timer_setup(struct timer *base, int prescaler, int reload)
{
	base->PSC = prescaler;
	base->ARR = reload;
	base->CR1 &= ~TIM_CR1_DIR_BIT;	/* Direction upward */

	/* TODO: explicitly setup mode */
}

