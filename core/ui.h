#ifndef CORE_UI_H
#define CORE_UI_H

#include <stdbool.h>
#include <lvgl/lvgl.h>

/* Block any user actions and reset timer */
void ui_lock(bool lock);
void ui_init(void);

#endif

