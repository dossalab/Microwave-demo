#ifndef SCHED_H
#define SCHED_H

#define MAX_TASKS	10

#include <stdint.h>

void tasks_init(uint32_t ticks);
int tasks_set(int interval, void (*handler)(void), int oneshot);
void tasks_proceed(void);

#endif

