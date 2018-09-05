// 
// 
// 

#include "thTimer.h"


void ThTimer::init()
{


}


// ThTimer ThTimer;

// File thTimer

/*
**********************************************************************************************
*  Function: initializeTimers()
*/
// Call this at starup and after 30 days so as to gracefully reste all the timers before an overflow occurs.
// Caution:  Please add any other timers that are created to this routine so they are also reset.
void ThTimer::initializeTimers(void) {

	noInterrupts();     // Turn off interrupts

	// Shift All Timers to new time base of Zero
	previousClk_ms -= timer0_millis;
	previousClk125_ms -= timer0_millis;
	previousClk250_ms -= timer0_millis;
	previousTempClk_ms -= timer0_millis;
	previousOneSecClk_ms -= timer0_millis;
	previousTwoSecClk_ms -= timer0_millis;
	timer0_millis = 0L;   // this is the global Arduino timer being reset to zero

	interrupts();      // reenable interrupts

}

