// File:  Relay.cpp
// By:  Mike Cipolla
// Date:  10/06/2015

#include "Relay.h"
#include "TStat.h"
#include "TemperatureControl.h"

void RelayClass::init()
{
	PORTA &= RELAY_OFF;   // Turn 4 LSBs low on PortA (Arduino Pins D25, D24, D23, D22)
	DDRA  |= RELAY_MASK;    // Define Arduino Pins D25, D24, D23, D22 as digital outputs
}

/*
**********************************************************************************************
*  Function: commandRelays()
*  Output discreet command signals to the relays to control furnas functions
*/
void RelayClass::commandRelays(u8 relayCommand)
{
	int i;
	


	switch (relayCommand){

	case CMD_COOL: {
		relayOut = RELAY_COOL;  // DEBUG
		PORTA |= SIG_GREEN;
		PORTA |= SIG_YELLOW;
		PORTA |= SIG_ORANGE;
		PORTA &= ~SIG_WHITE;
		break;
	}
	case CMD_HEAT: {
		relayOut = RELAY_HEAT;  // DEBUG
		PORTA |= SIG_GREEN;
		PORTA |= SIG_YELLOW;
		PORTA &= ~SIG_ORANGE;
		PORTA &= ~SIG_WHITE;
		break;
	}
	case CMD_EMR_HEAT: {
		relayOut = RELAY_EMR_HEAT;  // DEBUG
		PORTA |= SIG_GREEN;
		PORTA |= SIG_YELLOW;
		PORTA &= ~SIG_ORANGE;
		PORTA |= SIG_WHITE;
		break;
	}
	default:
	case CMD_IDLE: {
		if (tempControl.getSysFanOn())
		{
			relayOut = RELAY_FAN;  // DEBUG
			PORTA |= SIG_GREEN;
			PORTA &= ~SIG_YELLOW;
			PORTA &= ~SIG_ORANGE;
			PORTA &= ~SIG_WHITE;
		}
		else
		{
			relayOut &= RELAY_OFF;  // DEBUG
			PORTA &= RELAY_OFF;  // set 4 LSBs to zero
		}
		break;
	}
	}  // end switch


#ifdef SKIP
	u8 outputCmd = 0x0F & relayOut;
	Serial.print("rlOut = "); Serial.print(relayOut); Serial.print(" rCmd = "); Serial.print(relayCommand);
	Serial.println();
#endif

}

int RelayClass::status(void) 
{
	return relayOut;
}

RelayClass Relay;

