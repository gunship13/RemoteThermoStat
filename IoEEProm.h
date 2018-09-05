// IoEEProm.h

#ifndef _IOEEPROM_h
#define _IOEEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#include <EEPROM.h>
#else
	#include "WProgram.h"
#endif
#include "TemperatureControl.h"

class IoEEPromClass
{
 protected:

 public:
	void init();
	void readEE2Local(char*);
	void writeLocal2EE(char*);
	boolean areBuffsSame(char*, char*);
};

extern IoEEPromClass IoEEProm;

#endif

