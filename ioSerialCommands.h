// ioSerialCommands.h

#ifndef _IOSERIALCOMMANDS_h
#define _IOSERIALCOMMANDS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
//	#include "WString.h"
	#include "TemperatureControl.h"
#else
	#include "WProgram.h"
#endif

#define IO_LENGTH 20

class IoSerialCommandsClass
{
 protected:
	 boolean ioPortOpen;
	 char ioString[IO_LENGTH];
	 String cmdType;
	 String cmdValue;
	 

 public:
	void init(void);
	String readPortString(void);
	void parseCommand(String str);

};

extern IoSerialCommandsClass IoSerialCommands;

#endif

