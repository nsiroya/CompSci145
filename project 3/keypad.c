 /*
 * keypad.c
 *
 * Created: 5/12/2021 11:19:53 PM
 *  Author: jbyon
 */ 

#include <avr/io.h>
#include "keypad.h"
#include "avr.h"
#include "lcd.h"
#include <stdio.h>

void kp_init(){
	DDRC = 0x00; // Set all pins on GPIO C into inputs
	PORTC = 0x00; // Set all pins into no-connect state.
}

char kp_isPressed(char r, char c){
	DDRC = 0x00; // Set all pins on GPIO C into inputs
	PORTC = 0x00; // Set all pins into no-connect state.
	SET_BIT(DDRC, r);
	SET_BIT(PORTC, (4 + c));
	avr_wait(1);
	if(GET_BIT(PINC, (4 + c))){
		return 0;
	}
	else{
		avr_wait(1);
		return 1;
	}
}

char kp_getKey(){
	static char lastPressed = 0;
	char i, j;
	if (!lastPressed){
		for (i = 0; i < 4; i++){
			for (j = 0; j < 4; j++){
				if(kp_isPressed(i, j)){
					char currPressed = ((i*4)+j+1);
					if (lastPressed == currPressed){
						lastPressed = currPressed;
						return 0;
					}
					else {
						lastPressed = currPressed;
						return currPressed;
					}
				}
			}
		}
	}
	lastPressed = 0;
	return 0;
}