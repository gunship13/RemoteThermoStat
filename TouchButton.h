// TouchButton.h

#ifndef _TOUCHBUTTON_h
#define _TOUCHBUTTON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#ifndef __PRESUR
	#include <SeeedTouchScreen.h>
#endif
#include "TxBox.h"
#else
	#include "WProgram.h"
#endif
// #define DEBOUNCE 1        // Test Change 10/7/15
#define DEBOUNCE 0
// #define HOLD_THESHOLD  4  // Test Change 10/7/15
#define HOLD_THESHOLD  4
#define MAX_HOLD_COUNT 255  // for an 8 bit counter

class TouchButton: public TxBox
{
 protected:


 public:
	void init();
	bool ProcessXY(Point p);
	bool ButtonPressed(void);
	bool ButtonReleased(void);
	bool ButtonHeld(void);
//	TouchButton(INT16U ixp, INT16U iyp, INT16U ixw, INT16U iyw, INT16U ichColor, INT16U ibkColor, char *itxt) :
//		TxBox(ixp, iyp, ixw, iyw, ichColor, ibkColor, itxt);
	void DisplayButton(INT16U ichColor, INT16U ibkColor, char *itxt);
	TouchButton(INT16U ixp, INT16U iyp, INT16U ixw, INT16U iyw, INT16U ichColor, INT16U ibkColor, char *itxt);
private:
	INT16U xp;
	INT16U yp;
	INT16U xw;
	INT16U yw;
	INT8U  holdCount;  // Count how long button is pressed
	u8  touched;    // After debounce
	int8_t  oneShot;    // -1 is enabled, +1 when first touched, then cleared when touch event is used one time.
	u8  released;   // When touch is released
	u8  held;       // Set when button is pressed > HOLD_THRESHOLD
};


// extern TouchButtonClass TouchButton;

#endif

