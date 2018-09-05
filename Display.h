// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "TouchButton.h"
#include <SeeedTouchScreen.h>

// X is Horizontal Axis and Y is Vericle Axis
#define SCREEN_LEFT    0    // MinX - Display Left Side Pixel Value
#define SCREEN_RIGHT 240    // MaxX - Display Right Side Pixel Value
#define SCREEN_TOP     0    // MinY - Display Top Pixel Value
#define SCREEN_BOT   320    // MaxY - Diaplay Bottom Pixel Value

// Define additonal colors
#define PINK 0xFAAA		
#define RED1 0xf0e0

#define NUM_BUTTONS 11  // The Number of Key Buttons in User Interface, = Num Elements in SYS_BUTTONS
enum SYS_BUTTONS { COOL_UP, COOL_DN, HEAT_UP, HEAT_DN, HEAT, AUTO, COOL, OFF, FAN, EM_HEAT, END_BUT };
// Note: END_BUT is not a real button but is used to indicate no buttons are being returned.
	
class DisplayClass
{
 protected:
	 TouchButton		*pDispButtons;
	 TouchScreen		*pTS;
	 u8					NumDispButtons;
	 enum SYS_BUTTONS	TouchedButtonIndex;
	 enum SYS_BUTTONS	HeldButtonIndex;
	 enum SYS_BUTTONS	ReleaseButtonIndex;

	 void FindTouchedButton();  // find the index of the tourched button

 public:
	//void init(TouchScreen *pTScreen);
	void init();
	void write();
	void attachButtons(TouchButton *pButton, u8 NumButtons);
	void processButtons();
	void getTouch();
	enum SYS_BUTTONS   getTouchedButton();       // gets the index of the button touched
};

extern DisplayClass Display;

#endif

