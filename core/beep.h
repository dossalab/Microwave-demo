#ifndef CORE_BEEP_H
#define CORE_BEEP_H

#define BUZZER_TIMER		TIM2
#define BUZZER_TIMER_CH		2
#define RCC_BUZZER_TIMER	RCC_TIM2

#define BUZZER_PORT		GPIOA	/* TIMER 2 CH2 */
#define BUZZER_PIN		1

#define BEEP_LONG	100
#define BEEP_SHORT	50

void beep(int duration);
void beep_alarm(void);
void beep_stop(void);

void beep_init(void);

#endif

