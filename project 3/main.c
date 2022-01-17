/*
 * Givarvis_Project3_Music.c
 *
 * Created: 5/12/2021 2:52:32 PM
 * Author : jbyon
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"
#include "Note.h"
#include "keypad.h"

enum {
	OdeToJoy, // First Song
	FurElise, // Second Song
	Entertainer, // Third Song
	EmptyTrack4,
	EmptyTrack5,
	EmptyTrack6,
	EmptyTrack7,
	EmptyTrack8,
	EmptyTrack9
};

enum SM_MusicPlayer {
	SM_Init, SM_Paused, SM_Playing
} SMMP;


void processKey(char pressedKey, unsigned char* songIndex, unsigned int* i){

switch(pressedKey){
		case Song1:
			*songIndex = 0;
			*i = 0;
			break;
		case Song2:
			*songIndex = 1;
			*i = 0;
			break;
		case Song3:
			*songIndex = 2;
			*i = 0;
			break;
		case TempoUp:
			raiseTempo();
			break;
		case Song4:
			*songIndex = 3;
			*i = 0;
			break;
		case Song5:
			*songIndex = 4;
			*i = 0;
			break;
		case Song6:
			*songIndex = 5;
			*i = 0;
			break;
		case TempoDown:
			lowerTempo();
			break;
		case Song7:
			*songIndex = 6;
			*i = 0;
			break;
		case Song8:
			*songIndex = 7;
			*i = 0;
			break;
		case Song9:
			*songIndex = 8;
			*i = 0;
			break;
		case PitchUp:
			raiseOct();
			break;
		case Pause:
		case Play:
			// These two possibility was already covered by the SM logic.
			// Hence, do nothing.
			break;
		case PitchDown:
			lowerOct();
			break;
		default:
			// Do nothing.
			break;
	}
}

void printState (const char bPlaying, const char songIndex){
	static char lastIndex = 0;
	static char lastBool = 0;
	if (songIndex != lastIndex || bPlaying != lastBool) lcd_clr();
	char buf[16];
	if (bPlaying) sprintf(buf, "Now Playing: ");
	else sprintf(buf, "Paused: ");
	lcd_pos(0,0);
	lcd_puts(buf);
	switch (songIndex){
		case OdeToJoy:
			sprintf(buf, "Ode to Joy");
			break;
		case FurElise:
			sprintf(buf, "Fur Elise");
			break;
		case Entertainer:
			sprintf(buf, "T.Entertainer");
			break;
		case EmptyTrack4:
			sprintf(buf, "Empty Track");
			break;
		case EmptyTrack5:
			sprintf(buf, "Empty Track");
			break;
		case EmptyTrack6:
			sprintf(buf, "Empty Track");
			break;
		case EmptyTrack7:
			sprintf(buf, "Empty Track");
			break;
		case EmptyTrack8:
			sprintf(buf, "Empty Track");
			break;
		case EmptyTrack9:
			sprintf(buf, "Empty Track");
			break;
		default:
			sprintf(buf, "Invalid Track");
			break;
	}
	lcd_pos(1,0);
	lcd_puts(buf);
	
	lastIndex = songIndex;
	lastBool = bPlaying;
}

void SM_Logic(){
	static unsigned char songIndex = 0;
	static unsigned int i = 0;
	
	const struct Note songODJ[] =
	{ // 1: Ode to Joy
		{E4,Q},
		{E4,Q},
		{F4,Q},
		{G4,Q},
		{G4,Q},
		{F4,Q},
		{E4,Q},
		{D4,Q},
		{C4,Q},
		{C4,Q},
		{D4,Q},
		{E4,Q},
		{E4,QD},
		{D4,E},
		{D4,H},
		{E4,Q},
		{E4,Q},
		{F4,Q},
		{G4,Q},
		{G4,Q},
		{F4,Q},
		{E4,Q},
		{D4,Q},
		{C4,Q},
		{C4,Q},
		{D4,Q},
		{E4,Q},
		{D4,QD},
		{C4,E},
		{C4,H},
		{D4,Q},
		{D4,Q},
		{E4,Q},
		{C4,Q},
		{D4,Q},
		{E4,E},
		{F4,E},
		{E4,Q},
		{C4,Q},
		{D4,Q},
		{E4,E},
		{F4,E},
		{E4,Q},
		{D4,Q},
		{C4,Q},
		{D4,Q},
		{R,H},
		{E4,Q},
		{E4,Q},
		{F4,Q},
		{G4,Q},
		{G4,Q},
		{F4,Q},
		{E4,Q},
		{D4,Q},
		{C4,Q},
		{C4,Q},
		{D4,Q},
		{E4,Q},
		{D4,QD},
		{C4,E},
		{C4,H}
	};
	const struct Note songFE[] =
	{ // 2: Fur Elise
		{E5,E},
		{DS5,E},
		{E5,E},
		{DS5,E},
		{E5,E},
		{B4,E},
		{D5,E},
		{C5,E},
		{A4,Q},
		{R,E},
		{C4,E},
		{E4,E},
		{A4,E},
		{B4,Q},
		{R,E},
		{E4,E},
		{GS4,E},
		{B4,E},
		{C5,Q},
		{R,E},
		{A4,E},
		{E5,E},
		{DS5,E},
		{E5,E},
		{DS5,E},
		{E5,E},
		{B4,E},
		{D5,E},
		{C5,E},
		{A4,Q},
		{R,E},
		{C4,E},
		{E4,E},
		{A4,E},
		{B4,Q},
		{R,E},
		{E4,E},
		{C5,E},
		{B4,E},
		{A4,Q},
		{R,Q},
		{R,Q}
	};
	const struct Note songEntnr[] =
	{ // 3: The Entertainer
		{D4,S},
		{DS4,S},
		{E4,S},
		{C5,E},
		{E4,S},
		{C5,E},
		{E3,S},
		{C5,QD},
		{C5,S},
		{D5,S},
		{DS5,S},
		{E5,S},
		{C5,S},
		{D5,S},
		{E5,E},
		{B4,S},
		{D5,E},
		{C5,QD},
		{D4,S},
		{DS4,S},
		{E4,S},
		{C5,E},
		{E4,S},
		{C5,E},
		{E3,S},
		{C5,QD},
		{A4,S},
		{G4,S},
		{FS4,S},
		{A4,S},
		{C5,S},
		{E5,E},
		{D5,S},
		{C5,S},
		{A4,S},
		{D5,QD},
		{D4,S},
		{DS4,S},
		{E4,S},
		{C5,E},
		{E4,S},
		{C5,E},
		{E3,S},
		{C5,QD},
		{C5,S},
		{D5,S},
		{DS5,S},
		{E5,S},
		{C5,S},
		{D5,S},
		{E5,E},
		{B4,S},
		{D5,E},
		{C5,QD},
		{C5,S},
		{D5,S},
		{E5,S},
		{C5,S},
		{D5,S},
		{E5,E},
		{C5,S},
		{D5,S},
		{C5,S},
		{E5,S},
		{C5,S},
		{D5,S},
		{E5,E},
		{C5,S},
		{D5,S},
		{C5,S},
		{E5,S},
		{C5,S},
		{D5,S},
		{E5,E},
		{B4,S},
		{D5,E},
		{C5,QD},
		{R,E},
	};
	const struct Note emptyTrack[]=
	{ // Empty Track
		{R, W}
	};
	
	const int songLen[] = { 62, 42, 78, 1, 1, 1, 1, 1, 1};
	const int songBPM[] = { 190, 136, 93, 0, 0, 0, 0, 0, 0};
	const struct Note* songs[]= { songODJ, songFE, songEntnr, emptyTrack, emptyTrack, emptyTrack, emptyTrack, emptyTrack, emptyTrack};
	
	char keyPress = kp_getKey();
	
	TCCR0 = (1<<CS01) | (1<<CS00); // Normal Timer Mode x1024
	
	switch(SMMP){
		case SM_Init:
			SMMP = SM_Paused;
			break;
		case SM_Paused:
			if(keyPress == Play) SMMP = SM_Playing;
			else SMMP = SM_Paused;
			break;
		case SM_Playing:
			if(keyPress == Pause) SMMP = SM_Paused;
			else SMMP =  SM_Playing;
			break;
		default:
			SMMP = SM_Init;
	}
	
	struct Note sample = {A4, W};
	
	switch(SMMP){
		case SM_Init:
			// This should never run, and is left empty as a result
			break;
		case SM_Paused:
			processKey(keyPress, &songIndex, &i);
			setBPM(songBPM[songIndex]);
			printState(0, songIndex);
			break;
		case SM_Playing:
			processKey(keyPress, &songIndex, &i);
			setBPM(songBPM[songIndex]);
			printState(1, songIndex);
			playNote((songs[songIndex])[i]);
			i++;
			if (i >= songLen[songIndex]) i = 0;
			break;
		default:
			SMMP = SM_Init;
	}
}

int main(void){
	
	SET_BIT(DDRB, 3);
	
	lcd_init(0);
	lcd_pos(0,0);
	
	sei();
	TIMSK=(1<<TOIE1);
	
	TCCR0 = (1<<CS01) | (1<<CS00); // Normal Timer Mode x1024
	
    /* Replace with your application code */
    while (1) 
    {
		SM_Logic();
    }
}

