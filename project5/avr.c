/**
 * avr.c
 * Copyright (C) 2001-2020, Tony Givargis
 */

#include "avr.h"

void
avr_wait(unsigned short timeInt)
{
	while (timeInt--) {
			TCCR0 = (1<<CS02) | (1<<CS00); /* Timer0, normal mode, *1024 prescalar */
			TCNT0 = 0xF7;  		/* Load TCNT0, count for apprx 1ms */
			while((TIFR&0x01)==0);  /* Wait for TOV0 to roll over */
			TIFR = 0x1;  		/* Clear TOV0 flag */
	}
	TCCR0 = 0;
	
}
