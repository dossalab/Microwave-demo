#include <stdio.h>
#include <stdint.h>

#include <core/ui.h>
#include <core/ui_ll.h>
#include <core/board.h>
#include <core/tasks.h>
#include <core/microwave.h>

void main(void)
{
	board_init_all();
	tasks_init(SYSCLK / 100000); /* 10 microsecond resolution */

	ui_ll_init();
	ui_init();
	microwave_init();

	while (1) {
		ui_ll_poll();
		tasks_proceed();
	}
}

