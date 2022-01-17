/*
 * alarmClock.c
 *
 * Created: 2021-06-11 오후 5:06:35
 * Author : group4 
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"
#include "keypad.h"

enum SM_1 {
  SM_Init,
  SM_Clock,
  SM_EditTime,
  SM_SeeAlarms,
  SM_EditAlarm,
  SM_TimerRun,
  SM_TimerPause,
  SM_TimerSetup
} SM_AlClk;


struct ts {
	signed char hr;
	signed char min;
	signed char sec;
	signed int msec;
};

void sm_init(){
  SM_AlClk = SM_Init;
}

struct ts gClockTime = {0, 0, 0, 0};
struct ts gEditClock = {0, 0, 0, 0};
struct ts gTimer = {0, 0, 0, 0};
struct ts gTimerBuf = {0, 0, 0, 0};
struct ts alarms[6] = {
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};
struct ts alarmBuf = {0,0,0,0};
unsigned char bFlagAlarm = 0;
unsigned char alarmNum = 0;

void verify_timeTick() {
	if (gClockTime.msec > 999){
		gClockTime.sec += 1;
		gClockTime.msec -= 1000;
	}
	if (gClockTime.sec > 59){
		gClockTime.min += 1;
		gClockTime.sec -= 60;
		
		if((gClockTime.hr == alarms[0].hr && gClockTime.min == alarms[0].min) ||
		(gClockTime.hr == alarms[1].hr && gClockTime.min == alarms[1].min) ||
		(gClockTime.hr == alarms[2].hr && gClockTime.min == alarms[2].min) ||
		(gClockTime.hr == alarms[3].hr && gClockTime.min == alarms[3].min) ||
		(gClockTime.hr == alarms[4].hr && gClockTime.min == alarms[4].min) ||
		(gClockTime.hr == alarms[5].hr && gClockTime.min == alarms[5].min) ){
      bFlagAlarm = 1;
      TCCR2 = (1 << CS21) | (1 << CS20);
      TCNT2 = 0x60;
    }
	}
	if (gClockTime.min > 59){
		gClockTime.hr += 1;
		gClockTime.min -= 60;
	}
	if (gClockTime.hr > 23){
		gClockTime.hr -= 24;
	}
}

void verify_timeTickNeg() {
	if (gTimer.msec < 0){
		gTimer.sec -= 1;
		gTimer.msec += 1000;
	}
	if (gTimer.sec < 0){
		gTimer.min -= 1;
		gTimer.sec += 60;
	}
	if (gTimer.min < 0){
		gTimer.hr -= 1;
		gTimer.min += 60;
	}
	if (gTimer.hr < 0){
    SM_AlClk = SM_TimerPause;
		gTimer.hr = 0;
    gTimer.min = 0;
    gTimer.sec = 0;
    gTimer.msec = 0;

    bFlagAlarm = 1;
    TCCR2 = (1 << CS21) | (1 << CS20);
    TCNT2 = 0x60;
	}
}

void printClock(){
	char buf[16];
	sprintf(buf, "Clock Mode:");
	lcd_print(buf, 0, 0);
	sprintf(buf, "%02d:%02d:%02d", gClockTime.hr, gClockTime.min, gClockTime.sec);
	lcd_print(buf, 1, 0);
}

void printTimer(char bPaused){
	char buf[16];
	if(bPaused) sprintf(buf, "Timer: *Paused*");
	else sprintf(buf, "Timer: ");
	lcd_print(buf, 0, 0);
	sprintf(buf, "%02d:%02d:%02d", gTimer.hr, gTimer.min, gTimer.sec);
	lcd_print(buf, 1, 0);
}

void editClock(const char keyPress){
	char buf[16];
	static signed char currPos = 0;
	unsigned char numVal = 255;
	switch(keyPress){
	case num1:
		numVal = 1;
		break;
	case num2:
		numVal = 2;
		break;
	case num3:
		numVal = 3;
		break;
	case num4:
		numVal = 4;
		break;
	case num5:
		numVal = 5;
		break;
	case num6:
		numVal = 6;
		break;
	case num7:
		numVal = 7;
		break;
	case num8:
		numVal = 8;
		break;
	case num9:
		numVal = 9;
		break;
	case num0:
		numVal = 0;
		break;
	case charC:
		currPos += 1;
		currPos %= 6;
		break;
	case charD:
		currPos -= 1;
		if (currPos < 0) currPos += 6;
		break;
	default:
		// Do nothing
		break;
	}
  if(numVal != 255){
    switch(currPos){
    case 0: // Seconds, lower digit
      gEditClock.sec = (gEditClock.sec / 10) * 10 + numVal;
      break;
    case 1: // Seconds, upper digit
      if(numVal < 6) gEditClock.sec = (numVal) * 10 + gEditClock.sec % 10;
      break;
    case 2: // Minutes, lower digit
      gEditClock.min = (gEditClock.min / 10) * 10 + numVal;
      break;
    case 3: // Minutes, upper digit
      if(numVal < 6) gEditClock.min = (numVal) * 10 + gEditClock.min % 10;
      break;
    case 4: // Hours, lower digit
      if((numVal < 4 && gEditClock.hr / 10 == 2)) gEditClock.hr = (gEditClock.hr / 10) * 10 + numVal;
      break;
    case 5: // Hours, upper digit
      if((numVal < 2) || (numVal == 2 && gEditClock.hr % 10 < 4)) gEditClock.hr = (numVal) * 10 + gEditClock.hr % 10;
      break;
    }
  }
	
	sprintf(buf, "Edit Clock:");
	lcd_print(buf, 0, 0);
	sprintf(buf, "%02d:%02d:%02d", gEditClock.hr, gEditClock.min, gEditClock.sec);
	lcd_print(buf, 1, 0);
	
	switch(currPos){
	case 0: // Seconds, lower digit
    lcd_pos(1, 7);
		break;
	case 1: // Seconds, upper digit
    lcd_pos(1, 6);
		break;
	case 2: // Minutes, lower digit
    lcd_pos(1, 4);
		break;
	case 3: // Minutes, upper digit
    lcd_pos(1, 3);
		break;
	case 4: // Hours, lower digit
    lcd_pos(1, 1);
		break;
	case 5: // Hours, upper digit
    lcd_pos(1, 0);
		break;
	}
}

void seeAlarms(const char keyPress){
	switch(keyPress){
	case down:
		if(alarmNum == 5) alarmNum -= 5;
		else alarmNum += 1;
		break;
	case up:
		if(alarmNum == 0) alarmNum += 5;
		else alarmNum -= 1;
		break;
	}
	
	char buf[16];
	sprintf(buf, "Al%01d: %02d:%02d", alarmNum, alarms[alarmNum].hr, alarms[alarmNum].min);
	lcd_print(buf, 0, 0);
	sprintf(buf, "Al%01d: %02d:%02d", (alarmNum+1)%6, alarms[(alarmNum+1)%6].hr, alarms[(alarmNum+1)%6].min);
	lcd_print(buf, 1, 0);
	lcd_pos(0, 12);
}

void editAlarm(const char keyPress){
	char buf[16];
	static signed char currPos = 0;
	unsigned char numVal = 255;
	switch(keyPress){
	case num1:
		numVal = 1;
		break;
	case num2:
		numVal = 2;
		break;
	case num3:
		numVal = 3;
		break;
	case num4:
		numVal = 4;
		break;
	case num5:
		numVal = 5;
		break;
	case num6:
		numVal = 6;
		break;
	case num7:
		numVal = 7;
		break;
	case num8:
		numVal = 8;
		break;
	case num9:
		numVal = 9;
		break;
	case num0:
		numVal = 0;
		break;
	case charC:
		currPos += 1;
		currPos %= 4;
		break;
	case charD:
		currPos -= 1;
		if (currPos < 0) currPos += 4;
		break;
	default:
		// Do nothing
		break;
	}
  if(numVal != 255){
    switch(currPos){
    case 0: // Minutes, lower digit
      alarmBuf.min = (alarmBuf.min / 10) * 10 + numVal;
      break;
    case 1: // Minutes, upper digit
      if(numVal < 6) alarmBuf.min = (numVal) * 10 + alarmBuf.min % 10;
      break;
    case 2: // Hours, lower digit
      if((numVal < 4 && alarmBuf.hr / 10 == 2)) alarmBuf.hr = (alarmBuf.hr / 10) * 10 + numVal;
      break;
    case 3: // Hours, upper digit
      if((numVal < 2) || (numVal == 2 && alarmBuf.hr % 10 < 4)) alarmBuf.hr = (numVal) * 10 + alarmBuf.hr % 10;
      break;
    }
  }
	
	sprintf(buf, "Edit Alarm:");
	lcd_print(buf, 0, 0);
	sprintf(buf, "Al#%01d: %02d:%02d", alarmNum, alarmBuf.hr, alarmBuf.min);
	lcd_print(buf, 1, 0);
	
	switch(currPos){
	case 0: // Minutes, lower digit
    lcd_pos(1, 10);
		break;
	case 1: // Minutes, upper digit
    lcd_pos(1, 9);
		break;
	case 2: // Hours, lower digit
    lcd_pos(1, 7);
		break;
	case 3: // Hours, upper digit
    lcd_pos(1, 6);
		break;
	}
}

void setTimer(const char keyPress){
	char buf[16];
	static signed char currPos = 0;
	unsigned char numVal = 255;
	switch(keyPress){
	case num1:
		numVal = 1;
		break;
	case num2:
		numVal = 2;
		break;
	case num3:
		numVal = 3;
		break;
	case num4:
		numVal = 4;
		break;
	case num5:
		numVal = 5;
		break;
	case num6:
		numVal = 6;
		break;
	case num7:
		numVal = 7;
		break;
	case num8:
		numVal = 8;
		break;
	case num9:
		numVal = 9;
		break;
	case num0:
		numVal = 0;
		break;
	case charC:
		currPos += 1;
		currPos %= 6;
		break;
	case charD:
		currPos -= 1;
		if (currPos < 0) currPos += 6;
		break;
	default:
		// Do nothing
		break;
	}
  if(numVal != 255){
    switch(currPos){
    case 0: // Seconds, lower digit
      gTimerBuf.sec = (gTimerBuf.sec / 10) * 10 + numVal;
      break;
    case 1: // Seconds, upper digit
      if(numVal < 6) gTimerBuf.sec = (numVal) * 10 + gTimerBuf.sec % 10;
      break;
    case 2: // Minutes, lower digit
      gTimerBuf.min = (gTimerBuf.min / 10) * 10 + numVal;
      break;
    case 3: // Minutes, upper digit
      if(numVal < 6) gTimerBuf.min = (numVal) * 10 + gTimerBuf.min % 10;
      break;
    case 4: // Hours, lower digit
      if((numVal < 4 && gTimerBuf.hr / 10 == 2)) gTimerBuf.hr = (gTimerBuf.hr / 10) * 10 + numVal;
      break;
    case 5: // Hours, upper digit
      if((numVal < 2) || (numVal == 2 && gTimerBuf.hr % 10 < 4)) gTimerBuf.hr = (numVal) * 10 + gTimerBuf.hr % 10;
      break;
    }
  }
	
	sprintf(buf, "Set Timer:");
	lcd_print(buf, 0, 0);
	sprintf(buf, "%02d:%02d:%02d", gTimerBuf.hr, gTimerBuf.min, gTimerBuf.sec);
	lcd_print(buf, 1, 0);
	
	switch(currPos){
	case 0: // Seconds, lower digit
    lcd_pos(1, 7);
		break;
	case 1: // Seconds, upper digit
    lcd_pos(1, 6);
		break;
	case 2: // Minutes, lower digit
    lcd_pos(1, 4);
		break;
	case 3: // Minutes, upper digit
    lcd_pos(1, 3);
		break;
	case 4: // Hours, lower digit
    lcd_pos(1, 1);
		break;
	case 5: // Hours, upper digit
    lcd_pos(1, 0);
		break;
	}
}

void SM_Logic(){

  const char keyPress = kp_getKey();

  switch(SM_AlClk){
  case SM_Init:
    SM_AlClk = SM_Clock;
    break;
  case SM_Clock:
    switch (keyPress){
    case view:
      SM_AlClk = SM_SeeAlarms;
      lcd_clr();
      break;
    case editMode:
      SM_AlClk = SM_EditTime;
      gEditClock.hr = gClockTime.hr;
      gEditClock.min = gClockTime.min;
      gEditClock.sec = gClockTime.sec;
      lcd_clr();
      break;
    case modeTmr:
      SM_AlClk = SM_TimerSetup;
      lcd_clr();
      break;
    default:
      SM_AlClk = SM_Clock;
      break;
    }
    break;
  case SM_EditTime:
    switch(keyPress){
    case cancel:
			SM_AlClk = SM_Clock;
			lcd_clr();
      break;
		case confirm:
			gClockTime.hr = gEditClock.hr;
			gClockTime.min = gEditClock.min;
			gClockTime.sec = gEditClock.sec;
			SM_AlClk = SM_Clock;
			lcd_clr();
      break;
    default:
      SM_AlClk = SM_EditTime;
      break;
		}
    break;
  case SM_SeeAlarms:
    switch(keyPress){
    case cancel:
			SM_AlClk = SM_Clock;
			lcd_clr();
      break;
		case select:
			alarmBuf.hr = alarms[alarmNum].hr;
			alarmBuf.min = alarms[alarmNum].min;
			alarmBuf.sec = alarms[alarmNum].sec;
			SM_AlClk = SM_EditAlarm;
			lcd_clr();
      break;
    default:
      SM_AlClk = SM_SeeAlarms;
      break;
		}
    break;
  case SM_EditAlarm:
    switch(keyPress){
    case cancel:
			SM_AlClk = SM_SeeAlarms;
			lcd_clr();
      break;
		case confirm:
			alarms[alarmNum].hr = alarmBuf.hr;
			alarms[alarmNum].min = alarmBuf.min;
			alarms[alarmNum].sec = alarmBuf.sec;
			SM_AlClk = SM_SeeAlarms;
			lcd_clr();
      break;
    default:
      SM_AlClk = SM_EditAlarm;
      break;
		}
    break;
  case SM_TimerPause:
    switch (keyPress)
    {
    case editMode:
      SM_AlClk = SM_TimerSetup;
      gTimerBuf.hr = gTimer.hr;
      gTimerBuf.min = gTimer.min;
      gTimerBuf.sec = gTimer.sec;
      lcd_clr();
      break;
    case select:
      SM_AlClk = SM_TimerRun;
      break;
    case modeClk:
      SM_AlClk = SM_Clock;
      gTimer.hr = 0;
      gTimer.min = 0;
      gTimer.sec = 0;
      gTimer.msec = 0;
      lcd_clr();
      break;
    default:
      SM_AlClk = SM_TimerPause;
      break;
    }
    break;
  case SM_TimerRun:
    switch (keyPress)
    {
    case select:
      SM_AlClk = SM_TimerPause;
      break;
    default:
      SM_AlClk = SM_TimerRun;
      break;
    }
    break;
  case SM_TimerSetup:
    switch (keyPress)
    {
    case modeClk:
      SM_AlClk = SM_Clock;
      lcd_clr();
      break;
    case modeTmr:
      SM_AlClk = SM_TimerSetup;
      lcd_clr();
      break;
    case confirm:
      SM_AlClk = SM_TimerRun;
      gTimer.hr = gTimerBuf.hr;
      gTimer.min = gTimerBuf.min;
      gTimer.sec = gTimerBuf.sec;
      lcd_clr();
      break;
    default:
      SM_AlClk = SM_TimerSetup;
      lcd_clr();
      break;
    }
    break;
  default:
    SM_AlClk = SM_Clock;
    break;
  }


  switch(SM_AlClk){
  case SM_Init:
    lcd_print("Debug", 0, 0);
    lcd_print("SM_Init", 1, 0);
    break;
  case SM_Clock:
    printClock();
    break;
  case SM_EditTime:
    editClock(keyPress);
    break;
  case SM_SeeAlarms:
    seeAlarms(keyPress);
    break;
  case SM_EditAlarm:
    editAlarm(keyPress);
    break;
  case SM_TimerRun:
    printTimer(0);
    break;
  case SM_TimerPause:
    printTimer(1);
    break;
  case SM_TimerSetup:
    setTimer(keyPress);
    break;
  default:
    lcd_print("Debug:", 0, 0);
    lcd_print("default", 1, 0);
    break;
  }

  if (bFlagAlarm) if(keyPress == silence){
    bFlagAlarm = 0;
    TCCR2 = 0;
  }
}

ISR(TIMER1_OVF_vect){
	gClockTime.msec += 16;
	verify_timeTick(&gClockTime);
	if(SM_AlClk == SM_TimerRun){
		gTimer.msec -= 16;
		verify_timeTickNeg(gTimer);
	}
	TCNT1 = 0xFF82;  		// Load TCNT0, count for apprx 16ms
}

ISR(TIMER2_OVF_vect){
  TCNT2 = 0x60;
  if (GET_BIT(PORTB, 3)) CLR_BIT(PORTB, 3);
  else SET_BIT(PORTB, 3);
}

int main(void)
{
  SET_BIT(DDRB, 3);
  
  lcd_init(0);
  lcd_pos(0,0);
  kp_init();
  sm_init();
	
  sei();

  TIMSK = (1<<TOIE1) | (1<<TOIE2);
	
  TCCR0 = (1<<CS01) | (1<<CS00); // Normal Timer Mode x1024
  TCCR1B = (1<<CS12) | (1<<CS10); // Normal Timer Mode x1024

  TCNT1 = 0xFF82;

  while (1) {
    SM_Logic();
    avr_wait(100);
  }
}

