/*
 * LittlevGL porting layer
 */

#ifndef CORE_UI_LL_H
#define CORE_UI_LL_H

#include <lvgl/lvgl.h>

#define UI_LL_WIDTH		240
#define UI_LL_HEIGHT		240

/* 9600 bytes */
#define UI_LL_BUFFER_LINES	20

void ui_ll_keypad_group_add(lv_obj_t *obj);

void ui_ll_poll(void);
void ui_ll_init(void);

#endif

