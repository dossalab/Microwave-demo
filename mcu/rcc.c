#include <mcu/rcc.h>
#include <lib/bits.h>

/* CR bits */
/* External crystal generator enable / ready */
#define RCC_CR_HSEON_BIT	BIT(16)
#define RCC_CR_HSERDY_BIT	BIT(17)

/* Main PLL enable / ready*/
#define RCC_CR_PLLON_BIT	BIT(24)
#define RCC_CR_PLLRDY_BIT	BIT(25)

/* CFGR bits */
#define RCC_CFGR_PLLSRC_BIT	BIT(16)

struct rcc_block{
	volatile uint32_t *domain;
	uint32_t bitmask;
};

static struct rcc_block rcc_blocks[] = {
	[RCC_AFIO] = {&RCC->APB2ENR,  BIT(0) },
	[RCC_GPIOA] = {&RCC->APB2ENR, BIT(2) },
	[RCC_GPIOB] = {&RCC->APB2ENR, BIT(3) },
	[RCC_GPIOC] = {&RCC->APB2ENR, BIT(4) },
	[RCC_GPIOD] = {&RCC->APB2ENR, BIT(5) },

	/* SPI */
	[RCC_SPI1] = {&RCC->APB2ENR, BIT(12) },

	/* Timers */
	[RCC_TIM2] = {&RCC->APB1ENR, BIT(0) },
	[RCC_TIM3] = {&RCC->APB1ENR, BIT(1) },
	[RCC_TIM4] = {&RCC->APB1ENR, BIT(2) },
	[RCC_TIM5] = {&RCC->APB1ENR, BIT(3) }
};

void rcc_enable_line(enum rcc_line line)
{
	*rcc_blocks[line].domain |= rcc_blocks[line].bitmask;
}

void rcc_hse_enable(void)
{
	RCC->CR |= RCC_CR_HSEON_BIT;

	/* TODO: implement timeout */
	while (!(RCC->CR & RCC_CR_HSERDY_BIT));
}

/*
 * Launch PLL from HSE (8  Mhz) and switch SYSCLK source to it
 *
 * PLL multiplyer is 9
 * SYSCLK is 8 * 9 = 72 Mhz
 */
void rcc_setup_clocks(void)
{
	rcc_hse_enable();

	/* Set multiplyer (9) */
	RCC->CFGR &= ~(0b1111 << 18);
	RCC->CFGR |=  (0b0111 << 18);

	/* Clock source is HSE */
	RCC->CFGR |= RCC_CFGR_PLLSRC_BIT;

	/* Setup flash latency (wait states). 2 should do it for 72 Mhz */
	FLASH->ACR &= ~(0b111 << 0);
	FLASH->ACR |=  (0b010 << 0);
	
	/* APB1 requires 36 Mhz max. Also ADC is slow but it has its own div */
	RCC->CFGR &= ~(0b111 << 8);
	RCC->CFGR |=  (0b100 << 8); /* Divide by 2 */

	RCC->CR |= RCC_CR_PLLON_BIT;

	/* TODO: implement timeout */
	while (!(RCC->CR & RCC_CR_PLLRDY_BIT));

	/* PLL is launched. Hooray! Switch SYSCLK */
	RCC->CFGR &= ~(0b11 << 0);
	RCC->CFGR |=  (0b10 << 0);
}

