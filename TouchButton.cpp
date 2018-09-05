// File: TouchButton.cpp
// By: Mike Cipolla
// Date: 10/06/2015


#include "TouchButton.h"
#define DEBUG

void TouchButton::init()
{


}


TouchButton::TouchButton(INT16U ixp, INT16U iyp, INT16U ixw, INT16U iyw, INT16U ichColor, INT16U ibkColor, char *itxt) :
TxBox(ixp, iyp, ixw, iyw, ichColor, ibkColor, itxt)
{
	xp = ixp;
	yp = iyp;
	xw = ixw;
	yw = iyw;
	holdCount = 0;
	touched   = false;
	oneShot   = -1;  // enable leading edge trigger
	held      = false;

#ifdef DEBUG
	Serial.print("TouchButton Constructor");
	Serial.println();
#endif

};

void TouchButton::DisplayButton( INT16U ichColor, INT16U ibkColor, char *itxt){
	TxBox(xp, yp, xw, yw, ichColor, ibkColor, itxt);
}

bool TouchButton::ProcessXY(Point p)
{
#ifdef SKIP
	Serial.print("Top PXY yp= "); Serial.print(yp);
	Serial.println();
#endif
	if ((p.x > xp) && (p.x < xp + xw) && (p.y > yp) && (p.y < yp + yw))
	{
#ifdef SKIP
		Serial.print("if PXY");
		Serial.println();
#endif
		released = false;
		if(holdCount < MAX_HOLD_COUNT)
			holdCount++;
		if (holdCount > HOLD_THESHOLD){
			touched = false; // Removal Test 10/6/2015
			held = true;
		}
		else if (holdCount > DEBOUNCE)
		{ 
			//touched = true;
			oneShot = -oneShot;  // If leding edge (-1) is emanbel, then allow one touch.
			if (oneShot > 0)
				touched = true;
			else
				touched = false;
		}
#ifdef DEBUG
		Serial.print("held = "); Serial.print((int)held); Serial.print(" touched = "); Serial.print((int)touched);
		Serial.print(" oneShot = "); Serial.print(oneShot); Serial.println();
#endif
		
		return true;
	}
#ifdef SKIP
	Serial.print("*if* held = "); Serial.print((int)held); Serial.print(" touched = "); Serial.print((int)touched);
	Serial.println();
#endif

	// The touch is not withthin the zone of this button
	if (touched || held ){
		released = true;
		// touched = false;   // Added: Test 10/6/15
#ifdef SKIP
		Serial.print("*released*");
		Serial.println();
#endif
	}
#ifdef SKIP
	Serial.print("BOT PXY");
	Serial.println();
#endif
	holdCount = 0;
	held = false;
	touched = false;
	oneShot = -1;     // reenable one shot press detection

	return false;
}
	//TouchButton.ButtonPressed();



bool TouchButton::ButtonPressed(void)
{
	if (!touched)
		return false;
//	touched = false;  // ??? When, Why was this commented out?
	if (oneShot > 0)
		oneShot = 0;  // Only allow one touch to be processed per screen press
	return true;
}

bool TouchButton::ButtonReleased(void)
{
	if (!released)
		return false;
	released = false;
	return true;
}

bool TouchButton::ButtonHeld(void)
{
	if (!held)
		return false;
	return true;
}
//TouchButton TouchButton;

