#include "delay.h"

void delay(unsigned long cycles)
{
	volatile unsigned long n = cycles * 6500;
	while (n--);
}

