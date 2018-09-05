// File:  Display.cpp
// By:  Mike Cipolla
// Date:  10/08/2015

#include "Display.h"
#include <SPI.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>

#include "TemperatureControl.h"
#include "TStat.h"

#define DEBUG

// Set up global screen data

TouchScreen ts = TouchScreen(XP, YP, XM, YM);

TouchButton *p_tb1;


// void DisplayClass::init(TouchScreen *pTScreen)
void DisplayClass::init(void)
{
	pTS = &ts;
	pDispButtons = NULL;
	NumDispButtons = 0;

	/* Initileze The Display */
	Tft.TFTinit();				// Initilize the TFT Display

	// Print the static background for the disply menues	
	Tft.fillRectangle(0, 32, 65, 120, PINK);  // Pink Block to Enclose the "Heat" "+" and "-" Buttons
	TxBox(10, 32, 55, 20, BLACK, PINK, "Heat");

	Tft.fillRectangle(180, 32, 60, 120, CYAN);  // CYAN Block to Enclose the "Cool" "+" and "-" Buttons
	TxBox(182, 32, 46, 20, BLACK, CYAN, "Cool");

	// Display the Version # at the bottom of the display
//1113	TxBox(10, 290, 220, 20, BLACK, WHITE, "TStat 01.10");
	TxBox(10, 290, 220, 20, BLACK, WHITE, "RTStat v01.00");
	delay(1000);   // Show Version for One second
	// Display Oprating Mode and Fan Status
	
//1113	TxBox(10, 260, 220, 20, BLACK, YELLOW, strcat(tempControl.getSysModeTx(), tempControl.getSysFanTx()));
	TxBox(10, 290, 220, 20, BLACK, YELLOW, strcat(tempControl.getSysModeTx(), tempControl.getSysFanTx()));
	static TouchButton tb1[NUM_BUTTONS] = {								       //   ENUM
		TouchButton(190, 60, 40, 40, WHITE, RED, "+"),    // Cool +			        COOL_UP
		TouchButton(190, 104, 40, 40, WHITE, BLUE, "-"),  // Cool -			        COOL_DN
		TouchButton(15, 60, 40, 40, WHITE, RED, "+"),     // Heat +			        HEAT_UP
		TouchButton(15, 104, 40, 40, WHITE, BLUE, "-"),   // Heat -			        HEAT_DN
		//  Make the Mode Buttons
		TouchButton(15, 150, 60, 40, WHITE, RED, ""),     // Heat Mode Sellect      HEAT
		TouchButton(80, 150, 60, 40, WHITE, GREEN, ""),   // Auto Mode Sellect      AUTO
		TouchButton(145, 150, 60, 40, WHITE, BLUE, ""),   // Cool Mode Sellect      COOL
		TouchButton(15, 196, 60, 40, BLACK, WHITE, ""),   // Off  Mode Sellect      OFF
		TouchButton(80, 196, 60, 40, BLACK, GRAY1, ""),   // Fan  Manual Sellect    FAN
		TouchButton(145, 196, 60, 40, WHITE, RED, ""),    // Emergency Heat Sellect EM_HEAT
		TouchButton(MAX_X, MAX_Y, 0, 0, BLACK, BLACK, "") // Dummy Button for End   END_BUT
};
	// Add text on top of the mode buttons
	TxBox(15, 160, 60, 20, WHITE, RED, "Heat");			// Heat Mode Sellect
	TxBox(80, 160, 60, 20, WHITE, GREEN, "Auto");		// Auto Mode Sellect
	TxBox(145, 160, 60, 20, WHITE, BLUE, "Cool");		// Cool Mode Sellect
	TxBox(15, 206, 60, 20, BLACK, WHITE, "Off");		// Off  Mode Sellect
	TxBox(80, 206, 60, 20, BLACK, GRAY1, "Fan");		// Fan  Manual Sellect
	TxBox(145, 206, 60, 20, WHITE, RED, "EHeat");		// Emergency Heat Sellect

	p_tb1 = tb1;  // Initialize the value of the pointer to the Touch Buttons in tb1.
	Display.attachButtons(tb1, NUM_BUTTONS);
#ifdef SKIP
	Serial.print("tb1 = "); Serial.print((u16)tb1);
	Serial.println();
#endif

}

void DisplayClass::processButtons()
{
	getTouch();

#ifdef SKIP
	Serial.print("processButtonIndex = "); Serial.print(myTouchButton);
	Serial.println();
#endif
	switch (TouchedButtonIndex){
	default:
	case END_BUT:
#ifdef SKIP
		Serial.print("Error processButtonIndex = "); Serial.print(myTouchButton);
		Serial.println();
#endif
		break;
		// Process the Temperature UP/DOWN Buttons Here
	case COOL_UP:
		(p_tb1+COOL_UP)->DisplayButton(GRAY1, BRIGHT_RED, "+");
		tempControl.incrementHighTemp();  // Added for Test 10/7/15
		break;
	case COOL_DN:
		(p_tb1 + COOL_DN)->DisplayButton(GRAY1, 0x071f, "-");
		tempControl.decrementHighTemp();  // Added for Test 10/7/15
		break;
	case HEAT_UP:
		(p_tb1 + HEAT_UP)->DisplayButton(GRAY1, BRIGHT_RED, "+");
		tempControl.incrementLowTemp();
		break;
	case HEAT_DN:
		(p_tb1 + HEAT_DN)->DisplayButton(GRAY1, 0x071f, "-");
		tempControl.decrementLowTemp();
		break;

	// Process the Mode Control Buttons Here
	case HEAT:
		//(p_tb1 + HEAT)->DisplayButton(YELLOW, 0x071f, "Ht");// BRIGHT_BLUE
		tempControl.setSysMode(SYS_HEAT);
		break;
	case AUTO:
		//(p_tb1 + AUTO)->DisplayButton(YELLOW, 0x071f, "Au");// BRIGHT_BLUE
		tempControl.setSysMode(SYS_AUTO);
		break;
	case COOL:
		//(p_tb1 + COOL)->DisplayButton(YELLOW, 0x071f, "Co");// BRIGHT_BLUE
		tempControl.setSysMode(SYS_COOL);
		break;
	case OFF:
		//	(p_tb1 + OFF)->DisplayButton(YELLOW, 0x071f, "Of");// BRIGHT_BLUE
		tempControl.setSysMode(SYS_OFF);
		break;
	case FAN:
		//(p_tb1 + 8)->DisplayButton(YELLOW, 0x071f, "Fa");// BRIGHT_BLUE
		tempControl.toggleSysFan();
		break;
	case EM_HEAT:
		//(p_tb1 + EM_HEAT)->DisplayButton(YELLOW, 0x071f, "EH");// BRIGHT_BLUE
		tempControl.setSysMode(SYS_EMR_HEAT);
		break;
	}

	if (TouchedButtonIndex == END_BUT) {
	//	??? Need this?
	}

	switch (HeldButtonIndex){
	default:
		break;
	// Process the Repeating Buttons for Temperatre UP/DOWN Here
	case COOL_UP:
		(p_tb1)->DisplayButton(GRAY2, BRIGHT_RED, "+");
		tempControl.incrementHighTemp();
		break;
	case COOL_DN:
		(p_tb1 + COOL_DN)->DisplayButton(GRAY2, 0x071f, "-");// BRIGHT_BLUE
		tempControl.decrementHighTemp();
		break;
	case HEAT_UP:
		(p_tb1 + HEAT_UP)->DisplayButton(GRAY2, BRIGHT_RED, "+");
		tempControl.incrementLowTemp();
		break;
	case HEAT_DN:
		(p_tb1 + HEAT_DN)->DisplayButton(GRAY2, 0x071f, "-");// BRIGHT_BLUE
		tempControl.decrementLowTemp();
		break;
	//case HEAT:
	//	//(p_tb1 + HEAT)->DisplayButton(YELLOW, 0x071f, "Ht");// BRIGHT_BLUE
	//	tempControl.setSysMode(SYS_HEAT);
	//	break;
	//case AUTO:
	//	//(p_tb1 + AUTO)->DisplayButton(YELLOW, 0x071f, "Au");// BRIGHT_BLUE
	//	tempControl.setSysMode(SYS_AUTO);
	//	break;
	//case COOL:
	//	//(p_tb1 + COOL)->DisplayButton(YELLOW, 0x071f, "Co");// BRIGHT_BLUE
	//	tempControl.setSysMode(SYS_COOL);
	//	break;
	//case OFF:
	////	(p_tb1 + OFF)->DisplayButton(YELLOW, 0x071f, "Of");// BRIGHT_BLUE
	//	tempControl.setSysMode(SYS_OFF);
	//	break;
	////case FAN:
	////	//(p_tb1 + FAN)->DisplayButton(YELLOW, 0x071f, "Fa");// BRIGHT_BLUE
	////	tempControl.toggleSysFan();
	////	break;
	//case EM_HEAT:
	//	//(p_tb1 + EM_HEAT)->DisplayButton(YELLOW, 0x071f, "EH");// BRIGHT_BLUE
	//	tempControl.setSysMode(SYS_EMR_HEAT);
	//	break;

	}

	switch (ReleaseButtonIndex){
	case END_BUT:
	default:
		ReleaseButtonIndex = END_BUT;
		break;
	case COOL_UP:
		(p_tb1+COOL_UP)->DisplayButton(WHITE, RED, "+");
		break;
	case COOL_DN:
		(p_tb1 + COOL_DN)->DisplayButton(WHITE, BLUE, "-");
		break;

	case HEAT_UP:
		(p_tb1 + HEAT_UP)->DisplayButton(WHITE, RED, "+");
		break;
	case HEAT_DN:
		(p_tb1 + HEAT_DN)->DisplayButton(WHITE, BLUE, "-");
		break;

	}
	if (ReleaseButtonIndex == END_BUT) {
// ??? is this needed
	}
}
void DisplayClass::write()
{
	char sTemp[25];

	sprintf(sTemp, "%d F", tempControl.getHighTemp());
	TxBox(74, 45, 105, 40, BLACK, CYAN, sTemp);
	
	sprintf(sTemp, "%d F", tempControl.getLowTemp());
	TxBox(63, 95, 105, 40, BLACK, PINK, sTemp);

	// Display Oprating Mode
	
//1113	TxBox(10, 260, 220, 20, BLACK, YELLOW, strcat(tempControl.getSysModeTx(), tempControl.getSysFanTx()));
	TxBox(10, 290, 220, 20, BLACK, YELLOW, strcat(tempControl.getSysModeTx(), tempControl.getSysFanTx()));

	if (tempControl.getRunCommand() == CMD_EMR_HEAT)
//1113		TxBox(200, 260, 30, 20, RED, YELLOW, "EM");
		TxBox(200, 290, 30, 20, RED, YELLOW, "EM");

}

void DisplayClass::attachButtons(TouchButton *pButton, u8 NumButtons)  // Get pointer to array of buttons
{
	NumDispButtons = NumButtons;
	pDispButtons   = pButton;
}

void DisplayClass::getTouch()
{
	u8 i = 0;
	TouchButton *pTryButton;
	pTryButton = pDispButtons;
	TouchedButtonIndex = END_BUT;  // Use large value for non-index indication
	HeldButtonIndex = END_BUT;  // Use large value for non-index indication
	Point p = pTS->getPoint();

	if (p.z > __PRESURE) {  // Map the X and Y Touch Screen Values to X and Y Screen Pixels
		p.x = map(p.x, TS_MINX, TS_MAXX, SCREEN_LEFT, SCREEN_RIGHT);
		p.y = map(p.y, TS_MINY, TS_MAXY, SCREEN_TOP,  SCREEN_BOT);
	}
	else {
		p.x = 0;
		p.y = 0;
#ifdef SKIP
		Serial.print("p.x = "); Serial.print(p.x); Serial.print(" p.y = "); Serial.print(p.y);
		Serial.println();
#endif
		}


		while (i < NumDispButtons)  // check all the buttons
		{
			pTryButton->ProcessXY(p);
			if (pTryButton->ButtonPressed())
			{
				TouchedButtonIndex = (SYS_BUTTONS)i;
#ifdef SKIP
				Serial.print("TouchedButtonIndex = "); Serial.print(i);
				Serial.println();
#endif
				return;  // take first button pressed found
			}
			if (pTryButton->ButtonHeld())
			{
				HeldButtonIndex = (SYS_BUTTONS)i;
#ifdef DEBUG
				Serial.print(">>** HeldButtonIndex = "); Serial.print(i);
				Serial.println();
#endif
				return;  // take first button pressed found
			}
			if (pTryButton->ButtonReleased())
			{
				ReleaseButtonIndex = (SYS_BUTTONS)i;
#ifdef SKIP
				Serial.print("ReleaseButtonIndex = "); Serial.print(i);
				Serial.println();
#endif
				return;  // take first button pressed found
			}
			pTryButton++;
			i++;
		}
//	}
}


enum SYS_BUTTONS DisplayClass::getTouchedButton()       // gets the index of the button touched
{
	return TouchedButtonIndex;
}


DisplayClass Display;

