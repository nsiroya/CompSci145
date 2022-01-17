/**
 * Note.h
 *
 * Created: 5/12/2021 11:01:44 PM
 *  Author: jbyon
 */

#ifndef _Note_H_
#define _Note_H_

enum {
	R,
	C1,
	CS1,
	D1,
	DS1,
	E1,
	F1,
	FS1,
	G1,
	GS1,
	A1,
	B1,
	C2,
	CS2,
	D2,
	DS2,
	E2,
	F2,
	FS2,
	G2,
	GS2,
	A2,
	B2,
	C3,
	CS3,
	D3,
	DS3,
	E3,
	F3,
	FS3,
	G3,
	GS3,
	A3,
	B3,
	C4,
	CS4,
	D4,
	DS4,
	E4,
	F4,
	FS4,
	G4,
	GS4,
	A4,
	B4,
	C5,
	CS5,
	D5,
	DS5,
	E5,
	F5,
	FS5,
	G5,
	GS5,
	A5,
	B5,
	C6,
	CS6,
	D6,
	DS6,
	E6,
	F6,
	FS6,
	G6,
	GS6,
	A6,
	B6,
	C7,
	CS7,
	D7,
	DS7,
	E7,
	F7,
	FS7,
	G7,
	GS7,
	A7,
	B7,
};

enum {
	PH, // Placeholder - a 0 duration note inserted just to make the songs be of a uniform length
	S, // 16th note; a dotted sixteenth note was considered too impractical to implement, although it can be implemented.
	E, // 8th note;
	ED, // Dotted 8th
	Q, // Quarter Note
	QD, // Dotted Qrtr Note
	H, // Half note
	HD, // Dotted Half note
	W, // Whole note
	WD // Dotted Whole note
};

struct Note {
	char tone;
	char duration;
};

void playNote(const struct Note currNote);
void setBPM(const unsigned int songBPM);
void raiseOct();
void lowerOct();
void raiseTempo();
void lowerTempo();

#endif /* _Note_H_ */
