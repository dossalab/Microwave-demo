/*
 * Sample setup (Blue pill STM32F103 + ST7789 display
 *
 * Clocks:
 *   HSE (8Mhz) 72 Mhz system, 36 Mhz APB2
 *   GPIOA, GPIOB, GPIOC, SPI clocks enabled for simpilcity.
 *
 * LED:
 *   PC13
 *
 * SPI1:
 *   MOSI: PA7
 *   SCK: PA5
 *
 * ST7789 1.3" IPS screen connected to SPI1
 * Buttons:
 *   Up: PB13
 *   Down: PB12
 */

#ifndef CORE_BOARD_H
#define CORE_BOARD_H

#include <mcu/spi.h>
#include <mcu/gpio.h>
#include <drivers/st7789.h>

/*
 * Clocks are setup in board_init_all
 */
#define SYSCLK		(72000000UL)

extern struct st7789 st7789_handle;

#define LED_PORT		GPIOC
#define LED_PIN			13

#define BUTTON_PORT		GPIOB
#define BUTTON_OK_PIN		14
#define BUTTON_UP_PIN		13
#define BUTTON_DOWN_PIN		12

#define LCD_SPI			SPI1
#define LCD_SPI_RCC		RCC_SPI1
#define LCD_DC_PORT		GPIOA
#define LCD_DC_PIN		3
#define LCD_RESET_PORT		GPIOA
#define LCD_RESET_PIN		2
#define LCD_CS_PORT		GPIOA
#define LCD_CS_PIN		4
#define LCD_BL_PIN		15
#define LCD_BL_PORT		GPIOB

#define board_led_on()		gpio_clr(LED_PORT, LED_PIN)
#define board_led_off()		gpio_set(LED_PORT, LED_PIN)

#define board_lcd_blit(x0, y0, x1, y1, data) \
		st7789_blit(&st7789_handle, x0, y0, x1, y1, data)

#define board_lcd_bl_on()	gpio_set(LCD_BL_PORT, LCD_BL_PIN)
#define board_lcd_bl_off()	gpio_clr(LCD_BL_PORT, LCD_BL_PIN)

#define board_read_ok()		!gpio_get(BUTTON_PORT, BUTTON_OK_PIN)
#define board_read_up()		!gpio_get(BUTTON_PORT, BUTTON_UP_PIN)
#define board_read_down()	!gpio_get(BUTTON_PORT, BUTTON_DOWN_PIN)

void board_init_all(void);

#endif

