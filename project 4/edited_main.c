/*
 * voltMeter.c
 *
 * Created: 5/26/2021 3:07:21 PM
 * Author : jbyon
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"
#include "keypad.h"


enum SM_VltMtr { SM_VM_Init, SM_VM_Off, SM_VM_On } SM_VM;

int getSample(){
	ADMUX = (1 << REFS0) | (1 << MUX4);
	ADCSRA = (1 << ADEN) | (1 << ADSC);
	while(GET_BIT(ADCSRA, ADSC));
	ADCSRA = 0x00;
	return ADC;
}

float convertToVolts(int voltBus){
	float returnVal;
	returnVal = voltBus * 5.0 / 1023.0;
	return returnVal;
}

void voltMeterLogic(){
	static char keyPress;
	static char lastKeypress = 0;
	
	static unsigned long int cumulVolt = 0;
	static unsigned int counter = 0;
	unsigned int currVolt = 0;
	static unsigned int minVolt = 1023;
	static unsigned int maxVolt = 0; 
	
	char buf[5];
	
	SET_BIT(DDRB, 3);
	keyPress = kp_getKey();
	
	if (keyPress == lastKeypress){
		lastKeypress = keyPress;
		keyPress = 0;
	}
	else {
		lastKeypress = keyPress;
	}
	
	switch (SM_VM){
	case SM_VM_Init:
		SM_VM = SM_VM_Off;
		break;
	case SM_VM_Off:
		switch(keyPress){
		case kpA:
			SM_VM = SM_VM_On;
			break;
		default:
			SM_VM = SM_VM_Off;
		}
		break;
	case SM_VM_On:
		switch(keyPress){
		case kpB:
			SM_VM = SM_VM_Off;
			break;
		default:
			SM_VM = SM_VM_On;
			break;
		}
		break;
	default:
		SM_VM = SM_VM_Init;
		break;
	}
	
	// Regardless of State, reset if the reset key was pressed.
	if(keyPress == kpC){
		cumulVolt = 0;
		counter = 0;
		maxVolt = 0;
		minVolt = 1023;
	}
	
	
	switch (SM_VM){
		case SM_VM_Init:
		// empty block; this should never be entered normally
			break;
		case SM_VM_Off:
		// Do nothing;
			break;
		case SM_VM_On:
			currVolt = getSample();
			cumulVolt += currVolt;
			counter += 1;
			if(currVolt < minVolt) minVolt = currVolt;
			if(currVolt > maxVolt) maxVolt = currVolt;
			break;
		default:
		// This block should never be executed; even if it is, it will be rectified on the next run of the SM machine.
			break;
	}
	
	
	// Regardless of State: Display lcd;
	if(counter){
		sprintf(buf, "%4.2f", convertToVolts(currVolt)); // Instantaneous
		//sprintf(buf, "%4d", currVolt);
		lcd_print(buf, 0, 0);
		sprintf(buf, "%4.2f", convertToVolts(maxVolt)); // Maximum
		//sprintf(buf, "%4d", maxVolt);
		lcd_print(buf, 0, 8);
		sprintf(buf, "%4.2f", convertToVolts(minVolt)); // Minimum
		//sprintf(buf, "%4d", minVolt);
		lcd_print(buf, 1, 8);
		sprintf(buf, "%4.2f", ((cumulVolt) / (float) counter * 5.0 / 1023.0 )); // Average
		//sprintf(buf, "%4d", cumulVolt/counter);
		lcd_print(buf, 1, 0);
	}
	else{
		sprintf(buf, "-.--");
		lcd_print(buf, 0, 0);
		lcd_print(buf, 0, 8);
		lcd_print(buf, 1, 0);
		lcd_print(buf, 1, 8);
	}
}

int main(void){
	lcd_init();
	kp_init();
	
	
	for(;;){
		voltMeterLogic();
		avr_wait(500);
	}
	
	return 0;
}