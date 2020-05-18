#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIOA_BASE	0x40010800U /* GPIO Port A */
#define GPIOB_BASE	0x40010C00U /* GPIO Port B */
#define GPIOC_BASE	0x40011000U /* GPIO Port C */
#define GPIOD_BASE	0x40011400U /* GPIO Port D */

struct gpio {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
};

#define GPIOA	((struct gpio*)GPIOA_BASE)
#define GPIOB	((struct gpio*)GPIOB_BASE)
#define GPIOC	((struct gpio*)GPIOC_BASE)
#define GPIOD	((struct gpio*)GPIOD_BASE)

void gpio_in(struct gpio *bank, int pin);
void gpio_out(struct gpio *bank, int pin);
void gpio_af(struct gpio *bank, int pin);

void gpio_set(struct gpio *bank, int pin);
void gpio_clr(struct gpio *bank, int pin);
void gpio_toggle(struct gpio *bank, int pin);

int gpio_get(struct gpio *bank, int pin);

#endif

