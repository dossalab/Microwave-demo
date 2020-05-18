#include <mcu/gpio.h>
#include <core/ui.h>
#include <core/tasks.h>
#include <core/beep.h>

#define LIGHT_PORT	GPIOB
#define LIGHT_PIN	11
#define DOOR_PORT	GPIOB
#define DOOR_PIN	9
#define HEATER_PORT	GPIOC
#define HEATER_PIN	13

static bool door_open = 0;

static void heat_on(void)
{
	gpio_clr(HEATER_PORT, HEATER_PIN);
}

static void heat_off(void)
{
	gpio_set(HEATER_PORT, HEATER_PIN);
}

static void light_off(void)
{
	gpio_clr(LIGHT_PORT, LIGHT_PIN);
}

static void light_on(void)
{
	gpio_set(LIGHT_PORT, LIGHT_PIN);
}

static void microwave_read_door(void)
{
	door_open = gpio_get(DOOR_PORT, DOOR_PIN);

	if (door_open) {
		beep(BEEP_SHORT);
		heat_off();
		light_on();
		ui_lock(1);
	} else {
		light_off();
		ui_lock(0);
	}
}

int microwave_heat(int heat)
{
	if (heat > 0) {
		if (door_open) {
			/* Unable to heat - door is open! */
			return -1;
		} else {
			heat_on();
		}
	} else {
		heat_off();
	}

	return 0;
}

void microwave_init(void)
{
	gpio_in(DOOR_PORT, DOOR_PIN);
	gpio_out(HEATER_PORT, HEATER_PIN);
	gpio_out(LIGHT_PORT, LIGHT_PIN);

	tasks_set(100, microwave_read_door, 0);
}

