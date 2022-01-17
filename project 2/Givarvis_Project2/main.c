/*
 * Givarvis_Project2.c
 *
 * Created: 4/26/2021 3:43:16 PM
 * Author : jbyon
 */ 

#include "avr.h"
#include "lcd.h"
#include <stdio.h>


enum SM_Clock {
	SM_Init, SM_Unset, SM_Set
} SMClock;

struct dt {
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hr;
	unsigned int min;
	unsigned int sec;
};

void init_dt (struct dt* dateTime) {
	dateTime->year = 1970;
	dateTime->month = 1;
	dateTime->day = 1;
	dateTime->hr = 0;
	dateTime->min = 0;
	dateTime->sec = 0;
}

unsigned int getDays(struct dt* dateTime){
	if(dateTime->month == 1 || dateTime->month == 3 || dateTime->month == 5 || dateTime->month == 7 || dateTime->month == 8 || dateTime->month == 10 || dateTime->month == 12) return 31;
	else if (dateTime->month != 2) return 30;
	else {
		if (!(dateTime->year % 400)) return 29;
		else if(!(dateTime->year % 100)) return 28;
		else if(!(dateTime->year % 4)) return 29;
		else return 28;
	}
}

void verify_dt(struct dt* dateTime) {
	if (dateTime->sec > 59){
		dateTime->min += dateTime-> sec / 60;
		dateTime->sec %= 60;
	}
	if (dateTime->min > 59){
		dateTime->hr += dateTime-> min / 60;
		dateTime->min %= 60;
	}
	if (dateTime->hr > 23 ){
		dateTime->day += dateTime-> hr / 24;
		dateTime->hr %= 24;
	}
	while (dateTime->day > getDays(dateTime)){
		char temp = dateTime->day - (getDays(dateTime));
		dateTime->day = temp;
		++(dateTime->month);
	}
	while (dateTime->month > 12){
		dateTime->year += 1;
		dateTime->month -= 12;
	}
	if(dateTime->year > 9999){
		dateTime->year -= ((dateTime->year / 1970) - 1) * 1970;	
	}
}

void advance_dt(struct dt* dateTime) {
	(dateTime->sec) += 1;
	verify_dt(dateTime);
}

void print_dt(const struct dt* dateTime, unsigned char bMilitaryTime) {
	char buf[17];
	lcd_pos(0,0);
	sprintf(buf, "%02d/%02d/%04d", dateTime->month, dateTime->day, dateTime->year);
	lcd_puts(buf);
	lcd_pos(1,0); // Change rows
	if (bMilitaryTime) sprintf(buf, "%02d:%02d:%02d", dateTime->hr, dateTime->min, dateTime->sec); // Military Time
	else{ // Standard Time
		sprintf(buf, "%02d:%02d:%02d %s", (dateTime->hr % 12 == 0 ? 12 : dateTime->hr % 12), dateTime->min, dateTime->sec, (dateTime->hr > 12 ? "PM" : "AM"));
	}
	lcd_puts(buf);
}

void initSMClock(struct dt* dateTime){
	init_dt(dateTime);
	SMClock = SM_Init;
}

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
		avr_wait(1000);
		return 1;
		}
}


char kp_getKey(){	
	char i, j;
	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++){
			if(kp_isPressed(i, j)){
				return ((i*4)+j+1);
			}
		}
	}
	return 0;
}

void changeDT(struct dt* dateTime, unsigned char kpInput, unsigned char* currLoc, unsigned char* bMilitaryTime){
	// Date Time will be corrected to the proper time when exiting SM_Unset; this function lacks that functionality.
	if(kpInput == 0 || kpInput == 4 || kpInput == 8) return; // No Valid keypress was made. Exit function immediately when this occurs.
	else if(kpInput == 12){
		(*currLoc) = (*currLoc) == 13 ? 0 : (*currLoc) + 1; // if currLoc is index 13, wrap to 0. Otherwise, increment the index.
		// (dateTime has a total of 14 digits involved. 4 for year, 2 for day, 2 for month, 2 for hr, 2 for min, 2 for sec.
		// Therefore, using 0 indexing, this the highest possible index is 13.) 
		return;
	}
	else if(kpInput == 16){
		(*currLoc) = (*currLoc) ? (*currLoc) - 1 : 13; // if currLoc is a non-zero index, decrement one. Otherwise, wrap around to 13.
		return;
	}
	else if(kpInput == 13){
		(*bMilitaryTime) = 1;
		return;
	}
	else if(kpInput == 15){
		(*bMilitaryTime) = 0;
		return;
	}
	else{
		char numVal;
		// The math explained: ((kpInput % 4) + ((kpInput / 4) * 3))) can be broken up into 2 parts:
		// kpInput % 4, and (kpInput / 4) * 3.
		// for both parts, 4 is used in the division because every 4 values, a new row starts.
		// kpInput == 4 is A, which is used to enter SM_Unset, or dt Setting mode.
		// kpInput == 8 is B, which is used to enter SM_Set, or clock mode.
		// kpInput == 12, as we see above, moves the cursor 1 space higher in the dateTime
		// kpInput == 16, as we see above, moves the cursor 1 space lower in the dateTime
		// As we can see, all multiples of 4 are not actually used for inputting digits.
		// row 1 has 1, 2, 3, A; row 2 has 4, 5, 6, B, and so on. These have kpInput values 1, 2, 3, 4, 5, 6, 7, 8 respectively.
		// We can observe that doing (kpInput % 4) will consistently get values of 1, 2, 3
		// for the first three keys of each row, which are the numeric values.
		// We can also observe that doing (kpInput / 4) will output the row number, indexed to 0.
		// This row number, multiplied by 3 grants us the numeric offset we see on the keypad.
		// Adding these two numbers accurately gets the numeric values on the keypad for our use.
		// the condition if kpInput != 14, or rather, kpInput == 14 checks for if the value pressed was the 2nd key of rowIndex 3. (4th row)
		// this is the key for the number 0 on the keypad, hence the special rule for it.
		// 13 and 15 are used to change to 24hr and 12hr clocks.
		if (kpInput != 14) {numVal = ((kpInput % 4) + ((kpInput / 4) * 3));}
		else {numVal = 0;}
		switch((*currLoc)){
			case 0: // Seconds, lower digit
				dateTime->sec = (dateTime->sec / 10) * 10 + numVal;
				break;
			case 1: // Seconds, upper digit
				dateTime->sec = (numVal) * 10 + dateTime->sec % 10;
				break;
			case 2: // Minutes, lower digit
				dateTime->min = (dateTime->min / 10) * 10 + numVal;
				break;
			case 3: // Minutes, upper digit
				dateTime->min = (numVal) * 10 + dateTime->min % 10;
				break;
			case 4: // Hours, lower digit
				dateTime->hr = (dateTime->hr / 10) * 10 + numVal;
				break;
			case 5: // Hours, upper digit
				dateTime->hr = (numVal) * 10 + dateTime->hr % 10;
				break;
			case 6: // Year, year
				dateTime->year = (dateTime->year / 10) * 10 + numVal;
				break;
			case 7: // Year, decade
				dateTime->year = (dateTime->year / 100) * 100 + (numVal) * 10 + dateTime->year % 10;
				break;
			case 8: // Year, century
				dateTime->year = (dateTime->year / 1000) * 1000 + (numVal) * 100 + dateTime->year % 100;
				break;
			case 9: // Year, Millennium
				dateTime->year = (numVal) * 1000 + dateTime->year % 1000;
				break;
			case 10: // Day, lower digit
				dateTime->day = (dateTime->day / 10) * 10 + numVal;
				break;
			case 11: // Day, upper digit
				dateTime->day = (numVal) * 10 + dateTime->day % 10;
				break;
			case 12: // Month, lower digit
				dateTime->month = (dateTime->month / 10) * 10 + numVal;
				break;
			case 13: // Month, upper digit
				dateTime->month = (numVal) * 10 + dateTime->month % 10;
				break;
			default: // The program should never execute this; regardless, if this happens, it will merely reset the currLoc to index 0;
				(*currLoc) = 0;
		}
	}
}

void cursorPos(unsigned char currLoc){
	switch(currLoc){
		case 0:
			lcd_pos(1,7);
			break;
		case 1:
			lcd_pos(1,6);
			break;
		case 2:
			lcd_pos(1,4);
			break;
		case 3:
			lcd_pos(1,3);
			break;
		case 4:
			lcd_pos(1,1);
			break;
		case 5:
			lcd_pos(1,0);
			break;
		case 6:
			lcd_pos(0,9);
			break;
		case 7:
			lcd_pos(0,8);
			break;
		case 8:
			lcd_pos(0,7);
			break;
		case 9:
			lcd_pos(0,6);
			break;
		case 10:
			lcd_pos(0,4);
			break;
		case 11:
			lcd_pos(0,3);
			break;
		case 12:
			lcd_pos(0,1);
			break;
		case 13:
			lcd_pos(0,0);
			break;
		default: // The program should never execute this; regardless, if this happens, it will treat the cursor pos as if off screen;
			lcd_pos(0,39);
	}
}

void clockLogic(struct dt* dateTime){
	static unsigned char currLoc = 0;
	static unsigned char lastPress = 0;
	static unsigned char bMilitaryTime = 1;
	// Transition Logic
	switch(SMClock){
		case SM_Init:
			SMClock = SM_Set;
			break;
		case SM_Unset:
			if(lastPress == 8){
				verify_dt(dateTime);
				SMClock = SM_Set;
				lcd_init(SMClock);
				currLoc = 0;
			}
			else{
				SMClock = SM_Unset;
			}
			break;
		case SM_Set:
			if(lastPress == 4){
				SMClock = SM_Unset;
				lcd_init(SMClock);
				currLoc = 0;
			}
			else{
				SMClock = SM_Set;
			}
			break;
		default:
			// Error handling in case memory gets corrupted.
			init_dt(dateTime);
	}
	
	// State Logic
	switch(SMClock){
		case SM_Init:
			// This state logic is intentionally left empty as the program should never execute this.
			break;
		case SM_Unset:
			avr_wait(25-16);
			lastPress = kp_getKey();
			changeDT(dateTime, lastPress, &currLoc, &bMilitaryTime);
			print_dt(dateTime, bMilitaryTime);
			cursorPos(currLoc);
			break;
		case SM_Set:
			advance_dt(dateTime);
			avr_wait(1000-16);
			lastPress = kp_getKey();
			switch(lastPress){
				case 13: // Switch to Military Time
					bMilitaryTime = 1;
					lcd_init(SMClock);
					break;
				case 15: // Switch to 12-hour clock
					bMilitaryTime = 0;
					lcd_init(SMClock);
					break;
				default:
					// do nothing
					break;
			}
			// 1000 - 16 because of the avr_wait 1 inside kp_isPressed is called 16 times in kp_getKey,
			// adding up to 1000. Because of function overhead, this should actually overshoot 1 second by a
			// statistically significant amount so this is not actually a very accurate clock.
			print_dt(dateTime, bMilitaryTime);
			break;
		default:
			// This state logic is intentionally left empty as the program should never execute this.
			break;
	}
}

int main() { 
	struct dt dateTime;
	initSMClock(&dateTime);
	lcd_init(SMClock);
	kp_init();
	lcd_pos(0,0);
	for(;;){
		clockLogic(&dateTime);
	}
}