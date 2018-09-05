// File:  Relay.h
// By:  Mike Cipolla
// Date:  10/06/2015

#ifndef _RELAY_h
#define _RELAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/*
** We will use the following Digital Out Pins on the Mega Arduino
** Pin:      D25,  D24,  D23,  D22 corresponding to 
** Bits:       3,    2,    1,    0 on PORTA. Note we don't use  Bits 7, 6, 5, or 4
** Wire:   White,Orang,Yello,Green   // These are the wire colors for my Furnice
** PORTA = 0x0X where the 4 bits represented by X are 1 for Hi output and 0 for Low output
** DDRA  = 0x0F to signal PORTA Bits 3, 2, 1, 0 are outputs
*/
#define RELAY_PORT PORTA
#define RELAY_DDR  DDRA

// Interface to the Furnace is done with these relays
//
//                          Furnace Wire Connections
//                          Relay Hot (+28V) connects to Red          MODE Commands X = Closed, 0 = Open
//                          Relay Ground connects to Brown            Off Cool Heat EmrHeat Fan
#define RELAY1  0       //  Relay 1 Normally Open connects to Green     O   X    X    X       X
#define RELAY2  1       //  Relay 2 Normally Open connects to Yellow    O   X    X    X       O
#define RELAY3  2       //  Relay 3 Normally Open connects to Orange    O   X    O    O       O
#define RELAY4  3       //  Relay 4 Normally Open connects to White     O   0    0    X       0

// Define a Hex Codes to sellect each individual signal wire by the relay
#define SIG_NULL	   0X00				// Aruduino Mega Digital Pin# (These are 4LSBs of PORTA)
#define SIG_GREEN	   0x01             // D22
#define SIG_YELLOW     0x02				// D23
#define SIG_ORANGE     0x04				// D24
#define SIG_WHITE      0x08				// D25

// Form Relay Modes into Hex Codes that select which furnace control wires the relays connect to Hot (+28V)
// These are in to process of being replaced with single pin at a time setting and clearing and are only used
// for DEBUG at the pressent.
#define RELAY_COOL     (SIG_GREEN | SIG_YELLOW | SIG_ORANGE)	// 0x07  
#define RELAY_HEAT     (SIG_GREEN | SIG_YELLOW)					// 0x03
#define RELAY_EMR_HEAT (SIG_GREEN | SIG_YELLOW | SIG_WHITE)		// 0x0B
#define RELAY_FAN      (SIG_GREEN)								// 0x01

// The following are used to intilize the pins as outputs
#define RELAY_MASK     0x0F										// The 4 LSBs control the relays
#define RELAY_OFF      ~RELAY_MASK								// 0xF0

class RelayClass
{
 protected:
	 byte relayOut;  // ??? this is not needed anymore except for Debug

 public:
	void init();
	void commandRelays(u8 relyCommands);
	int status(void);
};

extern RelayClass Relay;

#endif

