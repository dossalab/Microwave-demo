#ifndef DRIVERS_ST7789_H
#define DRIVERS_ST7789_H

#include <stdint.h>
#include <mcu/spi.h>
#include <mcu/gpio.h>

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD

#define ST7789_WIDTH 240
#define ST7789_HEIGHT 240

#if ST7789_ROTATION == 0
	#define X_SHIFT 0
	#define Y_SHIFT 80
#elif ST7789_ROTATION == 1
	#define X_SHIFT 80
	#define Y_SHIFT 0
#elif ST7789_ROTATION == 2
	#define X_SHIFT 0
	#define Y_SHIFT 0
#elif ST7789_ROTATION == 3
	#define X_SHIFT 0
	#define Y_SHIFT 0
#endif

struct st7789 {
	int pin_dc;
	struct gpio *port_dc;
	int pin_cs;
	struct gpio *port_cs;
	int pin_reset;
	struct gpio *port_reset;

	struct spi *spi_handle;
};

void st7789_blit(struct st7789 *handle, int x0, int y0,
		int x1, int y1, uint16_t *image); 

void st7789_setpixel(struct st7789 *handle, int x, int y, uint16_t color);
void st7789_init(struct st7789 *handle, struct spi *spi,
		struct gpio *port_dc,
		int dc,
		struct gpio *port_reset,
		int reset,
		struct gpio *port_cs,
		int cs);

#endif

