#include <stdbool.h>
#include <mcu/gpio.h>
#include <mcu/timer.h>
#include <mcu/rcc.h>
#include <core/board.h>
#include <core/tasks.h>
#include <core/beep.h>

static int alarm_counter = 0;

static void beep_timer_stop(void)
{
	/* duty cycle: 0% */
	timer_compare_set(BUZZER_TIMER, BUZZER_TIMER_CH, 0); 
	timer_stop(BUZZER_TIMER);
}

void beep(int duration)
{
	/* duty cycle: 10% */
	timer_compare_set(BUZZER_TIMER, BUZZER_TIMER_CH, 10); 
	timer_start(BUZZER_TIMER);

	tasks_set(100 * duration, beep_timer_stop, 1);
}

static void beep_alarm_proceed(void)
{
	if (alarm_counter > 0) {
		beep(BEEP_LONG);
		alarm_counter--;
	}
}

void beep_alarm(void)
{
	alarm_counter = 10;
	tasks_set(100 * 300, beep_alarm_proceed, 0);
}

void beep_stop(void)
{
	alarm_counter = 0;
	beep_timer_stop();
}

void beep_init(void)
{
	rcc_enable_line(RCC_BUZZER_TIMER);

	gpio_out(BUZZER_PORT, BUZZER_PIN);
	gpio_af(BUZZER_PORT, BUZZER_PIN);

	timer_setup(BUZZER_TIMER, SYSCLK / 86000, 100);
	timer_setup_pwm(BUZZER_TIMER, BUZZER_TIMER_CH);
}

