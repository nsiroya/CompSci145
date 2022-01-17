/*
 * keypad.h
 *
 * Created: 5/12/2021 11:19:38 PM
 *  Author: jbyon
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

void kp_init();
char kp_isPressed(char r, char c);
char kp_getKey();

enum{
	zero, Song1, Song2, Song3, TempoUp,
	Song4, Song5, Song6, TempoDown,
	Song7, Song8, Song9, PitchUp,
	Pause, Dis, Play, PitchDown 
	};

#endif /* KEYPAD_H_ */