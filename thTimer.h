// thTimer.h

#ifndef _THTIMER_h
#define _THTIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// Global Data
// Timers
extern unsigned long previousClk_ms;
extern unsigned long previousClk125_ms;
extern unsigned long previousClk250_ms;
extern unsigned long previousOneSecClk_ms;
extern unsigned long previousTwoSecClk_ms;
extern unsigned long previousTempClk_ms;
// Define several timer durrations scaled to mili-seconds
extern const unsigned long time125_ms;
extern const unsigned long time250_ms;
extern const unsigned long timeOneSec;
extern const unsigned long timeTwoSec;
// 30 days is a time a little short of 50 days when the long timer will overflow
extern const unsigned long time30Days;
// External Delclarations
extern volatile unsigned long timer0_millis;


class ThTimer
{
 protected:


 public:
	void init();
	void initializeTimers(void);
};

//extern ThTimer ThTimer;

#endif

