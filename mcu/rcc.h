#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#define RCC_BASE	0x40021000U
#define FLASH_BASE	0x40022000U

struct rcc {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
};

struct flash {
	volatile uint32_t ACR;
};

#define RCC	((struct rcc*)RCC_BASE)
#define FLASH	((struct flash*)FLASH_BASE)

enum rcc_line {
	RCC_AFIO,
	RCC_GPIOA,
	RCC_GPIOB,
	RCC_GPIOC,
	RCC_GPIOD,

	RCC_SPI1,

	RCC_TIM2,
	RCC_TIM3,
	RCC_TIM4,
	RCC_TIM5
};

void rcc_enable_line(enum rcc_line line);

void rcc_hse_enable(void);
void rcc_setup_clocks(void);

#endif

