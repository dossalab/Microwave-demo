/*
 * Main UI module, all user interactions are handled here
 */

#include <core/ui.h>
#include <core/ui_ll.h>
#include <core/tasks.h>
#include <core/board.h>
#include <core/microwave.h>
#include <misc/common.h>
#include <core/beep.h>

static lv_style_t big_digit_label_style;

static int timer_min, timer_sec;
static void timer_dec(int sec);
static void timer_inc(int sec);

/* time widget */
static lv_obj_t *time_label;
static void ui_time_update(void);
static void ui_time_init(void);

/* heat bar */
static lv_obj_t *heat_label;
static void ui_heat_set(int heat);
static void ui_heat_meter_init(void);

static bool ui_locked = false;

static void timer_reset(void)
{
	timer_sec = 0;
	timer_min = 0;
	ui_time_update();
}

/*
 * Beautufully wraps seconds and minutes, like real timer
 */
static void timer_dec(int sec)
{
	timer_sec -= sec;

	if (timer_sec < 0) {
		timer_sec = 59;
		timer_min--;
	}

	if (timer_min < 0) {
		timer_min = 59;
	}
}

static void timer_inc(int sec)
{
	timer_sec += sec;

	if (timer_sec > 59) {
		timer_sec = 0;
		timer_min++;
	}

	if (timer_min > 59) {
		timer_min = 0;
	}
}

static void ui_style_init(void)
{
	// lv_theme_t *th = lv_theme_material_init(180, NULL);
	// lv_theme_set_current(th);
	lv_style_copy(&big_digit_label_style, &lv_style_plain);
	big_digit_label_style.text.font = &roboto_50_digits;
}

/* Widgets */
/* timer widget */
static void ui_time_update(void)
{
	lv_label_set_text_fmt(time_label, "%02d:%02d", timer_min, timer_sec);
}

static void ui_time_init(void)
{
	lv_obj_t *text = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(text, 20, 20);
	lv_label_set_text(text, LV_SYMBOL_BELL);

	time_label = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(time_label, 50, 20);
	lv_label_set_style(time_label, LV_LABEL_STYLE_MAIN,
					&big_digit_label_style);
	ui_time_update();
}

/* Heat bar */
static void ui_heat_set(int heat)
{
	lv_label_set_text_fmt(heat_label, "%02d% %%", heat);
}

static void ui_heat_meter_init(void)
{
	lv_obj_t *text = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(text, 20, 70);
	lv_label_set_text(text, LV_SYMBOL_POWER);

	heat_label = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(heat_label, 50, 70);
	lv_label_set_style(heat_label, LV_LABEL_STYLE_MAIN,
					&big_digit_label_style);

	ui_heat_set(10);
}

/* Heat amount button */
static void ui_heat_amount_init(void)
{
	lv_obj_t *btn = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_pos(btn, 10, 125);
	lv_obj_set_size(btn, 220, 50);

	lv_obj_t *label = lv_label_create(btn, NULL);
	lv_label_set_text(label, LV_SYMBOL_CHARGE " Мощность");

	void _event(lv_obj_t *obj, lv_event_t event)
	{
		switch (event) {
		case LV_EVENT_PRESSED:
			break;
		}
	}

	lv_obj_set_event_cb(btn, _event);
	ui_ll_keypad_group_add(btn);
}

/* Heat on button */
static void ui_heat_start_init(void)
{
	lv_obj_t *btn = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_pos(btn, 10, 180);
	lv_obj_set_size(btn, 220, 50);

	lv_obj_t *label = lv_label_create(btn, NULL);
	lv_label_set_text(label, LV_SYMBOL_POWER " Старт");

	void _event(lv_obj_t *obj, lv_event_t event)
	{
		switch (event) {
		case LV_EVENT_PRESSED:
			if (!ui_locked) {
				timer_inc(30);
				ui_time_update();
			}

			break;
		}
	}

	lv_obj_set_event_cb(btn, _event);
	ui_ll_keypad_group_add(btn);
}

static void ui_second_tick(void)
{
	if (timer_sec > 0 || timer_min > 0) {
		timer_dec(1);
		ui_time_update();

		if (microwave_heat(1) < 0) {
			/* Unable to heat! May be door is open */
			timer_reset();
		}
	} else {
		microwave_heat(0);
	}
}

/*
 * Create UI controls, setup required callbacks, etc
 * At this point lvgl is competely setup
 */
static void ui_layout_create(void)
{
	/* Order is important: constructors will add items to keypad group */
	ui_time_init();
	ui_heat_meter_init();
	ui_heat_amount_init();
	ui_heat_start_init();
}

void ui_lock(bool lock)
{
	if (lock) {
		timer_reset();
	}

	ui_locked = lock;
}

void ui_init(void)
{
	ui_style_init();
	ui_layout_create();

	tasks_set(100 * 1000, ui_second_tick, 0);
}


