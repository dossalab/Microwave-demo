/*
 * LittlevGL porting layer
 */

#include <stdint.h>
#include <core/board.h>
#include <core/tasks.h>
#include <core/ui_ll.h>
#include <misc/common.h>

static lv_disp_buf_t display_buffer_desc;
static lv_color_t display_buffer[LV_HOR_RES_MAX * UI_LL_BUFFER_LINES];
static lv_indev_t *keypad_indev;
static lv_group_t *keypad_group;

static void ui_ll_flush(lv_disp_drv_t *display, const lv_area_t *area,
						lv_color_t *data)
{
	board_lcd_blit(area->x1, area->y1, area->x2, area->y2, (uint16_t *)data);
	lv_disp_flush_ready(display);
}

void ui_ll_keypad_group_add(lv_obj_t *obj)
{
	lv_group_add_obj(keypad_group, obj);
}

static bool ui_ll_keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
	static uint32_t last_key;

	if (board_read_ok()) {
		data->state = LV_INDEV_STATE_PR;
		last_key = LV_KEY_ENTER;
	} else
	if (board_read_up()) {
		data->state = LV_INDEV_STATE_PR;
		last_key = LV_KEY_PREV;
	} else
	if (board_read_down()) {
		data->state = LV_INDEV_STATE_PR;
		last_key = LV_KEY_NEXT;
	} else {
		data->state = LV_INDEV_STATE_REL;
	}

	data->key = last_key;

	/* No more data to read */
	return false;
}

static void ui_ll_redraw_cb(lv_disp_drv_t *disp_drv, uint32_t time, uint32_t px)
{
	board_lcd_bl_on();

	/* disable this callback, we used it for backlight only */
	disp_drv->monitor_cb = NULL;
}

static void ui_ll_tick(void)
{
	lv_tick_inc(1);
}

void ui_ll_poll(void)
{
	lv_task_handler();
}

void ui_ll_init(void)
{
	lv_disp_drv_t disp_drv;
	lv_indev_drv_t indev_drv;

	lv_init();
	lv_disp_drv_init(&disp_drv);
	lv_disp_buf_init(&display_buffer_desc, display_buffer,
				NULL, ARRAY_SIZE(display_buffer));

	disp_drv.hor_res = UI_LL_WIDTH;
	disp_drv.ver_res = UI_LL_HEIGHT;
	disp_drv.flush_cb = ui_ll_flush;
	disp_drv.buffer = &display_buffer_desc;
	disp_drv.monitor_cb = ui_ll_redraw_cb;
	lv_disp_drv_register(&disp_drv);

	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_KEYPAD;
	indev_drv.read_cb = ui_ll_keypad_read;
	keypad_indev = lv_indev_drv_register(&indev_drv);

	keypad_group = lv_group_create();
	lv_indev_set_group(keypad_indev, keypad_group);

	tasks_set(50, ui_ll_tick, 0);
}

