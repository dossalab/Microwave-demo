#include <core/tasks.h>
#include <mcu/cortex.h> /* for systick */

static volatile uint32_t task_flags;

volatile struct task {
	unsigned int interval;
	unsigned int counter;
	void (*handler)(void);
	int oneshot;
} tasks[MAX_TASKS];

void systick_handler(void)
{
	for (int i = 0; i < MAX_TASKS; i++) {
		volatile struct task *t = &tasks[i];

		if (t->interval <= 0) {
			continue;
		}

		t->counter--;
		if (t->counter <= 0) {
			task_flags |= (1 << i);

			if (!t->oneshot) {
				t->counter += t->interval;
			} else {
				t->interval = -1;
			}
		}
	}
}

void tasks_init(uint32_t ticks)
{
	systick_init(ticks);
	nvic_enable_irq(NVIC_IRQ_SYSTICK);
}

int tasks_set(int interval, void (*handler)(void), int oneshot)
{
	__DISABLE_IRQ;

	for (int i = 0; i < MAX_TASKS; i++) {
		volatile struct task *t= &tasks[i];

		if (t->handler == handler) {
			t->interval = interval;
			t->counter = interval;
			t->oneshot = oneshot;
			return 0;
		}
	}

	for (int i = 0; i < MAX_TASKS; i++) {
		volatile struct task *t = &tasks[i];

		if (t->interval <= 0) {
			t->interval = interval;
			t->counter = interval;
			t->handler = handler;
			t->oneshot = oneshot;
			return 0;
		}
	}

	__ENABLE_IRQ;

	return -1;
}


void tasks_proceed(void)
{
	__DISABLE_IRQ;
	volatile uint32_t flags = task_flags;
	volatile uint32_t clear_flags = 0;
	__ENABLE_IRQ;

	for (int i = 0; i < MAX_TASKS; i++) {
		if (flags & (1 << i)) {
			volatile struct task *t = &tasks[i];
			t->handler();
			clear_flags |= (1 << i);
		}
	}

	__DISABLE_IRQ;
	task_flags &= ~clear_flags;
	__ENABLE_IRQ;
}

