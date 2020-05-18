#include <stddef.h>
#include <misc/delay.h>
#include <lib/bytes.h>
#include <drivers/st7789.h>

/**
 * enum st7789v_command - ST7789V display controller commands
 *
 * @PORCTRL: porch setting
 * @GCTRL: gate control
 * @VCOMS: VCOM setting
 * @VDVVRHEN: VDV and VRH command enable
 * @VRHS: VRH set
 * @VDVS: VDV set
 * @VCMOFSET: VCOM offset set
 * @PWCTRL1: power control 1
 * @PVGAMCTRL: positive voltage gamma control
 * @NVGAMCTRL: negative voltage gamma control
 *
 * The command names are the same as those found in the datasheet to ease
 * looking up their semantics and usage.
 *
 * Note that the ST7789V display controller offers quite a few more commands
 * which have been omitted from this list as they are not used at the moment.
 * Furthermore, commands that are compliant with the MIPI DCS have been left
 * out as well to avoid duplicate entries.
 */
enum st7789v_command {
	PORCTRL = 0xB2,
	GCTRL = 0xB7,
	VCOMS = 0xBB,
	VDVVRHEN = 0xC2,
	VRHS = 0xC3,
	VDVS = 0xC4,
	VCMOFSET = 0xC5,
	PWCTRL1 = 0xD0,
	PVGAMCTRL = 0xE0,
	NVGAMCTRL = 0xE1,
};

/* MIPI DCS commands */
enum {
	MIPI_DCS_NOP			= 0x00,
	MIPI_DCS_SOFT_RESET		= 0x01,
	MIPI_DCS_GET_DISPLAY_ID		= 0x04,
	MIPI_DCS_GET_RED_CHANNEL	= 0x06,
	MIPI_DCS_GET_GREEN_CHANNEL	= 0x07,
	MIPI_DCS_GET_BLUE_CHANNEL	= 0x08,
	MIPI_DCS_GET_DISPLAY_STATUS	= 0x09,
	MIPI_DCS_GET_POWER_MODE		= 0x0A,
	MIPI_DCS_GET_ADDRESS_MODE	= 0x0B,
	MIPI_DCS_GET_PIXEL_FORMAT	= 0x0C,
	MIPI_DCS_GET_DISPLAY_MODE	= 0x0D,
	MIPI_DCS_GET_SIGNAL_MODE	= 0x0E,
	MIPI_DCS_GET_DIAGNOSTIC_RESULT	= 0x0F,
	MIPI_DCS_ENTER_SLEEP_MODE	= 0x10,
	MIPI_DCS_EXIT_SLEEP_MODE	= 0x11,
	MIPI_DCS_ENTER_PARTIAL_MODE	= 0x12,
	MIPI_DCS_ENTER_NORMAL_MODE	= 0x13,
	MIPI_DCS_EXIT_INVERT_MODE	= 0x20,
	MIPI_DCS_ENTER_INVERT_MODE	= 0x21,
	MIPI_DCS_SET_GAMMA_CURVE	= 0x26,
	MIPI_DCS_SET_DISPLAY_OFF	= 0x28,
	MIPI_DCS_SET_DISPLAY_ON		= 0x29,
	MIPI_DCS_SET_COLUMN_ADDRESS	= 0x2A,
	MIPI_DCS_SET_PAGE_ADDRESS	= 0x2B,
	MIPI_DCS_WRITE_MEMORY_START	= 0x2C,
	MIPI_DCS_WRITE_LUT		= 0x2D,
	MIPI_DCS_READ_MEMORY_START	= 0x2E,
	MIPI_DCS_SET_PARTIAL_AREA	= 0x30,
	MIPI_DCS_SET_SCROLL_AREA	= 0x33,
	MIPI_DCS_SET_TEAR_OFF		= 0x34,
	MIPI_DCS_SET_TEAR_ON		= 0x35,
	MIPI_DCS_SET_ADDRESS_MODE	= 0x36,
	MIPI_DCS_SET_SCROLL_START	= 0x37,
	MIPI_DCS_EXIT_IDLE_MODE		= 0x38,
	MIPI_DCS_ENTER_IDLE_MODE	= 0x39,
	MIPI_DCS_SET_PIXEL_FORMAT	= 0x3A,
	MIPI_DCS_WRITE_MEMORY_CONTINUE	= 0x3C,
	MIPI_DCS_READ_MEMORY_CONTINUE	= 0x3E,
	MIPI_DCS_SET_TEAR_SCANLINE	= 0x44,
	MIPI_DCS_GET_SCANLINE		= 0x45,
	MIPI_DCS_SET_DISPLAY_BRIGHTNESS = 0x51,		/* MIPI DCS 1.3 */
	MIPI_DCS_GET_DISPLAY_BRIGHTNESS = 0x52,		/* MIPI DCS 1.3 */
	MIPI_DCS_WRITE_CONTROL_DISPLAY  = 0x53,		/* MIPI DCS 1.3 */
	MIPI_DCS_GET_CONTROL_DISPLAY	= 0x54,		/* MIPI DCS 1.3 */
	MIPI_DCS_WRITE_POWER_SAVE	= 0x55,		/* MIPI DCS 1.3 */
	MIPI_DCS_GET_POWER_SAVE		= 0x56,		/* MIPI DCS 1.3 */
	MIPI_DCS_SET_CABC_MIN_BRIGHTNESS = 0x5E,	/* MIPI DCS 1.3 */
	MIPI_DCS_GET_CABC_MIN_BRIGHTNESS = 0x5F,	/* MIPI DCS 1.3 */
	MIPI_DCS_READ_DDB_START		= 0xA1,
	MIPI_DCS_READ_DDB_CONTINUE	= 0xA8,
};

/* MIPI DCS pixel formats */
#define MIPI_DCS_PIXEL_FMT_24BIT	7
#define MIPI_DCS_PIXEL_FMT_18BIT	6
#define MIPI_DCS_PIXEL_FMT_16BIT	5
#define MIPI_DCS_PIXEL_FMT_12BIT	3
#define MIPI_DCS_PIXEL_FMT_8BIT		2
#define MIPI_DCS_PIXEL_FMT_3BIT		1

#define BIT(X) (1 << X)

#define MADCTL_BGR BIT(3) /* bitmask for RGB/BGR order */
#define MADCTL_MV BIT(5) /* bitmask for page/column order */
#define MADCTL_MX BIT(6) /* bitmask for column address order */
#define MADCTL_MY BIT(7) /* bitmask for page address order */

static inline void st7789_select(struct st7789 *handle)
{
	gpio_clr(handle->port_cs, handle->pin_cs);
}

static inline void st7789_unselect(struct st7789 *handle)
{
	gpio_set(handle->port_cs, handle->pin_cs);
}

static void st7789_spi_send(struct st7789 *handle, uint8_t *data, int amount)
{
	st7789_select(handle);
	spi_send(handle->spi_handle, data, amount);
	st7789_unselect(handle);
}

static void st7789_data(struct st7789 *handle, uint8_t data)
{
	gpio_set(handle->port_dc, handle->pin_dc);

	gpio_clr(handle->port_cs, handle->pin_cs);
	spi_send_u8(handle->spi_handle, data);
	gpio_set(handle->port_cs, handle->pin_cs);
}

static void st7789_cmd(struct st7789 *handle, uint8_t cmd)
{
	gpio_clr(handle->port_dc, handle->pin_dc);

	gpio_clr(handle->port_cs, handle->pin_cs);
	spi_send_u8(handle->spi_handle, cmd);
	gpio_set(handle->port_cs, handle->pin_cs);
}

static void st7789_area(struct st7789 *handle, int x0, int y0, int x1, int y1)
{
	 st7789_cmd(handle, ST7789_CASET);
	 st7789_data(handle, x0 >> 8);
	 st7789_data(handle, x0 & 0xFF);
	 st7789_data(handle, x1 >> 8);
	 st7789_data(handle, x1 & 0xFF);

	 st7789_cmd(handle, ST7789_RASET);
	 st7789_data(handle, y0 >> 8);
	 st7789_data(handle, y0 & 0xFF);
	 st7789_data(handle, y1 >> 8);
	 st7789_data(handle, y1 & 0xFF);

	 st7789_cmd(handle, ST7789_RAMWR);
}

void st7789_setpixel(struct st7789 *handle, int x, int y, uint16_t color)
{
	union u16_bytes temp;
	temp.u16 = cpu_to_be16(color);

	st7789_area(handle, x, y, x + 1, y + 1);

	st7789_data(handle, temp.bytes[0]);
	st7789_data(handle, temp.bytes[1]);
}

void st7789_blit(struct st7789 *handle, int x0, int y0,
		int x1, int y1, uint16_t *image)
{
	int w, h;
	int bytes;

	/* TODO: Can x0 be greater? */
	w = x1 - x0 + 1;
	h = y1 - y0 + 1;
	bytes = 2 * w * h;

	for (int i = 0; i < w * h; i++) {
		image[i] = cpu_to_be16(image[i]);
	}

	st7789_area(handle, x0, y0, x1, y1);
	gpio_set(handle->port_dc, handle->pin_dc);

	st7789_spi_send(handle, (uint8_t *)image, bytes);
}

void st7789_fill(struct st7789 *handle, int x, int y, int w, int h,
				uint16_t color)
{
	union u16_bytes temp;
	temp.u16 = cpu_to_be16(color);

	st7789_area(handle, x, y, x + w - 1, y + h - 1);

	for (int i = 0; i < w * h; i++) {
		st7789_data(handle, temp.bytes[0]);
		st7789_data(handle, temp.bytes[1]);
	}
}

static void st7789_hw_reset(struct st7789 *handle)
{
	gpio_clr(handle->port_reset, handle->pin_reset);
	delay(500);
	gpio_set(handle->port_reset, handle->pin_reset);
}

static void st7789_sw_reset(struct st7789 *handle)
{
	st7789_cmd(handle, ST7789_SWRESET);
	delay(150);
}

void st7789_sleepmode(struct st7789 *handle, int sleep)
{
	st7789_cmd(handle, sleep? ST7789_SLPIN : ST7789_SLPOUT);
	delay(500);
}

void st7789_set_rotation(struct st7789 *handle, int rotation)
{
	uint8_t madctl_par = 0;

	switch (rotation) {
	case 0:
		break;
	case 90:
		madctl_par |= (MADCTL_MV | MADCTL_MY);
		break;
	case 180:
		madctl_par |= (MADCTL_MX | MADCTL_MY);
		break;
	case 270:
		madctl_par |= (MADCTL_MV | MADCTL_MX);
		break;
	default:
		return;
	}

	st7789_cmd(handle, MIPI_DCS_SET_ADDRESS_MODE);
	st7789_data(handle, madctl_par);
}

// void st7789_init(struct st7789 *par, struct spi *spi,
// 		int dc, int reset, int cs)
// {
// 	par->pin_dc = dc;
// 	par->pin_cs = cs;
// 	par->pin_reset = reset;
// 	par->spi_handle = spi;
// 
// 	gpio_out(par->pin_cs);
// 	gpio_clr(par->pin_cs);
// 	gpio_out(par->pin_dc);
// 	gpio_out(par->pin_reset);
// 	gpio_set(par->pin_reset);
// 
//  	st7789_hw_reset(par);
// 	st7789_sw_reset(par);
// 
// 	/* turn off sleep mode */
// 	write_reg(par, MIPI_DCS_EXIT_SLEEP_MODE);
// 	mdelay(120);
// 
// 	/* set pixel format to RGB-565 */
// 	write_reg(par, MIPI_DCS_SET_PIXEL_FORMAT, MIPI_DCS_PIXEL_FMT_16BIT);
// 
// 	write_reg(par, PORCTRL, 0x08, 0x08, 0x00, 0x22, 0x22);
// 
// 	/*
// 	 * VGH = 13.26V
// 	 * VGL = -10.43V
// 	 */
// 	write_reg(par, GCTRL, 0x35);
// 
// 	/*
// 	 * VDV and VRH register values come from command write
// 	 * (instead of NVM)
// 	 */
// 	write_reg(par, VDVVRHEN, 0x01, 0xFF);
// 
// 	/*
// 	 * VAP =  4.1V + (VCOM + VCOM offset + 0.5 * VDV)
// 	 * VAN = -4.1V + (VCOM + VCOM offset + 0.5 * VDV)
// 	 */
// 	write_reg(par, VRHS, 0x0B);
// 
// 	/* VDV = 0V */
// 	write_reg(par, VDVS, 0x20);
// 
// 	/* VCOM = 0.9V */
// 	write_reg(par, VCOMS, 0x20);
// 
// 	/* VCOM offset = 0V */
// 	write_reg(par, VCMOFSET, 0x20);
// 
// 	/*
// 	 * AVDD = 6.8V
// 	 * AVCL = -4.8V
// 	 * VDS = 2.3V
// 	 */
// 	write_reg(par, PWCTRL1, 0xA4, 0xA1);
// 	write_reg(par, MIPI_DCS_SET_DISPLAY_ON);
// 
// 	write_reg(par, 0x21);
// }

void st7789_correct_gamma(struct st7789 *handle)
{
	uint8_t pgamma[] = { 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, \
				0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23 };

	uint8_t ngamma[] = { 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, \
				0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23 };

	st7789_cmd(handle, 0xE0);
	for (int i = 0; i < sizeof(pgamma); i++) {
		st7789_data(handle, pgamma[i]);
	}

	st7789_cmd(handle, 0xE1);
	for (int i = 0; i < sizeof(pgamma); i++) {
		st7789_data(handle, ngamma[i]);
	}
}

void st7789_init(struct st7789 *handle, struct spi *spi,
		struct gpio *port_dc,
		int dc,
		struct gpio *port_reset,
		int reset,
		struct gpio *port_cs,
		int cs)
{
	handle->port_dc = port_dc;
	handle->pin_dc = dc;

	handle->port_cs = port_cs;
	handle->pin_cs = cs;

	handle->port_reset = port_reset;
	handle->pin_reset = reset;

	handle->spi_handle = spi;

	gpio_out(handle->port_cs, handle->pin_cs);
	gpio_clr(handle->port_cs, handle->pin_cs);
	gpio_out(handle->port_dc, handle->pin_dc);
	gpio_out(handle->port_reset, handle->pin_reset);
	gpio_set(handle->port_reset, handle->pin_reset);

	st7789_hw_reset(handle);
	st7789_sw_reset(handle);

	st7789_sleepmode(handle, 0);

	st7789_cmd(handle, ST7789_COLMOD);
	st7789_data(handle, 0x55); /* 16-bit color */

	st7789_cmd(handle, ST7789_MADCTL);
	st7789_data(handle, 0x00); /* Row addr/col addr, bottom to top refresh */

	//st7789_area(handle, 0, 0, 240, 240);
	//

	st7789_cmd(handle, ST7789_CASET);
	st7789_data(handle, 0x00);
	st7789_data(handle, 0x00);
	st7789_data(handle, (240 + 0) >> 8);
	st7789_data(handle, (240 + 0) & 0xFF);

	st7789_cmd(handle, ST7789_RASET);
	st7789_data(handle, 0x00);
	st7789_data(handle, 0x00);
	st7789_data(handle, (240 + 0) >> 8);
	st7789_data(handle, (240 + 0) & 0xFF);

	st7789_correct_gamma(handle);
	st7789_cmd(handle, ST7789_INVON); /* Inversion on */
	delay(10);

	st7789_cmd(handle, ST7789_NORON); /* Normal mode on */
	delay(10);

	st7789_cmd(handle, ST7789_DISPON); /* Screen turn on */
	delay(10);

	st7789_set_rotation(handle, 2);
}

