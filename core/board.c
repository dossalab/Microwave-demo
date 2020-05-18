#include <core/board.h>
#include <core/beep.h>
#include <drivers/st7789.h>
#include <mcu/rcc.h>

struct st7789 st7789_handle;

static void board_spi_init(void)
{
	rcc_enable_line(LCD_SPI_RCC);

	gpio_out(GPIOA, 5);
	gpio_out(GPIOA, 7);

	gpio_af(GPIOA, 5);
	gpio_af(GPIOA, 7);

	spi_init(LCD_SPI);
}

static void board_display_init(void)
{
	st7789_init(&st7789_handle,
			LCD_SPI,
			LCD_DC_PORT, LCD_DC_PIN,
			LCD_RESET_PORT, LCD_RESET_PIN,
			LCD_CS_PORT, LCD_CS_PIN
	);
}

void board_init_all(void)
{
	rcc_setup_clocks();

	rcc_enable_line(RCC_AFIO);
	rcc_enable_line(RCC_GPIOA);
	rcc_enable_line(RCC_GPIOB);
	rcc_enable_line(RCC_GPIOC);

	gpio_out(LED_PORT, LED_PIN);
	board_led_off();

	gpio_in(BUTTON_PORT, BUTTON_UP_PIN);
	gpio_in(BUTTON_PORT, BUTTON_DOWN_PIN);

	gpio_out(LCD_BL_PORT, LCD_BL_PIN);
	board_lcd_bl_off();

	board_spi_init();
	board_display_init();

	beep_init();
}

