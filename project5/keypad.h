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
	unpressed,
	num1, num2, num3, charA, 
	num4, num5, num6, charB,
	num7, num8, num9, charC,
	star, num0, oct, charD
	};

enum{
	noButton,
	view, up, editMode, silence,
	left, kp6, right, modeClk,
	editField, down, kp11, modeTmr,
	cancel, kp14, confirm, select
	};

#endif /* KEYPAD_H_ */