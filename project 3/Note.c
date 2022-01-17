/*
 * Note.c
 *
 * Created: 5/12/2021 11:01:44 PM
 *  Author: jbyon
 */ 

#include "Note.h"
#include "avr.h"
#include "lcd.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile unsigned int beatCount = 0;
unsigned char tempo = 4;
signed char octave = 0;
unsigned int beatTCNT = 0;

ISR(TIMER1_OVF_vect) {
	beatCount += tempo;
	TCNT1 = beatTCNT;
}

void raiseOct(){
	if (octave < 2) octave += 1;
}

void lowerOct(){
	if (octave > -2) octave -= 1;
}

void raiseTempo(){
	if (tempo < 12) tempo++;
}

void lowerTempo(){
	if (tempo > 2) tempo--;
}

void setBPM(const unsigned int songBPM){
	switch(songBPM){
		case 93:
			beatTCNT = 0xFEC4;
			break;
		case 136:
			beatTCNT = 0xFF28;
			break;
		case 190:
			beatTCNT = 0xFF65;
			break;
		default:
			beatTCNT = 0xFFF0;
			// The specific BPM hasn't been programmed.
			// Play back at extreme speed
			// Refer to Excel sheet to program the value in.
	}
}

void playNote(const struct Note currNote){
	unsigned char noteFreq;
	unsigned char timerRate;
	unsigned int noteLen;
	
	unsigned char noteTone = currNote.tone + (12 * octave);
	
	switch (noteTone){
		case C1:
		noteFreq = 0x9B;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case CS1:
		noteFreq = 0xA0;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case D1:
		noteFreq = 0xA6;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case DS1:
		noteFreq = 0xAB;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case E1:
		noteFreq = 0xAF;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case F1:
		noteFreq = 0xB4;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case FS1:
		noteFreq = 0xB8;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case G1:
		noteFreq = 0xBC;
		timerRate = (1 << CS22) | (1 << CS21) | (1 << CS20); //x1024
		break;
		case GS1:
		noteFreq = 0x02;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case A1:
		noteFreq = 0x10;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case B1:
		noteFreq = 0x1E;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case C2:
		noteFreq = 0x2A;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case CS2:
		noteFreq = 0x36;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case D2:
		noteFreq = 0x41;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case DS2:
		noteFreq = 0x4C;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case E2:
		noteFreq = 0x56;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case F2:
		noteFreq = 0x60;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case FS2:
		noteFreq = 0x69;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case G2:
		noteFreq = 0x71;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case GS2:
		noteFreq = 0x79;
		timerRate = (1 << CS22) | (1 << CS21); //x256
		break;
		case A2:
		noteFreq = 0x02;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case B2:
		noteFreq = 0x10;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case C3:
		noteFreq = 0x1E;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case CS3:
		noteFreq = 0x2A;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case D3:
		noteFreq = 0x36;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case DS3:
		noteFreq = 0x41;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case E3:
		noteFreq = 0x4C;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case F3:
		noteFreq = 0x56;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case FS3:
		noteFreq = 0x60;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case G3:
		noteFreq = 0x69;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case GS3:
		noteFreq = 0x71;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case A3:
		noteFreq = 0x79;
		timerRate = (1 << CS22) | (1 << CS20); //x128
		break;
		case B3:
		noteFreq = 0x02;
		timerRate = (1 << CS22); //x64
		break;
		case C4:
		noteFreq = 0x10;
		timerRate = (1 << CS22); //x64
		break;
		case CS4:
		noteFreq = 0x1E;
		timerRate = (1 << CS22); //x64
		break;
		case D4:
		noteFreq = 0x2A;
		timerRate = (1 << CS22); //x64
		break;
		case DS4:
		noteFreq = 0x36;
		timerRate = (1 << CS22); //x64
		break;
		case E4:
		noteFreq = 0x41;
		timerRate = (1 << CS22); //x64
		break;
		case F4:
		noteFreq = 0x4C;
		timerRate = (1 << CS22); //x64
		break;
		case FS4:
		noteFreq = 0x56;
		timerRate = (1 << CS22); //x64
		break;
		case G4:
		noteFreq = 0x60;
		timerRate = (1 << CS22); //x64
		break;
		case GS4:
		noteFreq = 0x69;
		timerRate = (1 << CS22); //x64
		break;
		case A4:
		noteFreq = 0x71;
		timerRate = (1 << CS22); //x64
		break;
		case B4:
		noteFreq = 0x79;
		timerRate = (1 << CS22); //x64
		break;
		case C5:
		noteFreq = 0x02;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case CS5:
		noteFreq = 0x10;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case D5:
		noteFreq = 0x1E;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case DS5:
		noteFreq = 0x2A;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case E5:
		noteFreq = 0x36;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case F5:
		noteFreq = 0x41;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case FS5:
		noteFreq = 0x4C;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case G5:
		noteFreq = 0x56;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case GS5:
		noteFreq = 0x60;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case A5:
		noteFreq = 0x69;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case B5:
		noteFreq = 0x71;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case C6:
		noteFreq = 0x79;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case CS6:
		noteFreq = 0x80;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case D6:
		noteFreq = 0x88;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case DS6:
		noteFreq = 0x8E;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case E6:
		noteFreq = 0x95;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case F6:
		noteFreq = 0x9B;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case FS6:
		noteFreq = 0xA0;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case G6:
		noteFreq = 0xA6;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case GS6:
		noteFreq = 0xAB;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case A6:
		noteFreq = 0xAF;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case B6:
		noteFreq = 0xB4;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;
		case C7:
		noteFreq = 0xB8;
		timerRate = (1 << CS21) | (1 << CS20); //x32
		break;

	}
	switch(currNote.duration){
		case S:
			noteLen = 64;
			break;
		case E:
			noteLen = 128;
			break;
		case ED:
			noteLen = 192;
			break;
		case Q:
			noteLen = 256;
			break;
		case QD:
			noteLen = 384;
			break;
		case H:
			noteLen = 512;
			break;
		case HD:
			noteLen = 768;
			break;
		case W:
			noteLen = 1024;
			break;
		case WD:
			noteLen = 1536;
			break;
		default:
			noteLen = 0;
			break;
	}
	beatCount = 0;
	TCNT1 = beatTCNT;
	TCCR1B = (1<<CS12); // Normal Timer Mode x256
	
	while(beatCount < noteLen){
		if (currNote.tone != R && beatCount < (noteLen - 2 * tempo)){
			// The - tempo is introduced to provide an extremely small gap
			// to make notes of the same pitch played in a row be distinguishable as separate notes
			TCNT2 = noteFreq;
			TCCR2 = timerRate;
			while((GET_BIT(TIFR, 6)) == 0);
			SET_BIT(PORTB, 3);
			TCCR2 = 0;
			SET_BIT(TIFR, 6);
			TCNT2 = noteFreq;
			TCCR2 = timerRate;
			while((GET_BIT(TIFR, 6)) == 0);
			CLR_BIT(PORTB, 3);
			TCCR2 = 0;
			SET_BIT(TIFR, 6);
		}
		else{
			// Do nothing; you are resting.
		}
	}
}