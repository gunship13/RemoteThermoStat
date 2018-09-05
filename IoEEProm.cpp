// 
// 
// 

#include <EEPROM.h>
#include "IoEEProm.h"
#include "TemperatureControl.h"



void IoEEPromClass::init()
{
	

}

void IoEEPromClass::readEE2Local(char* pLocal)
{
	int i = 0;
	while (i++ < TC_DATA_SIZE)
	 *pLocal++ = EEPROM.read(i);
}


void IoEEPromClass::writeLocal2EE(char* pLocal)
{
	int i = 0;
	while (i++ < TC_DATA_SIZE)
		EEPROM.write(i, *pLocal++);
}


boolean IoEEPromClass::areBuffsSame(char* p1, char* p2)
{
	int i=0;
	while (i++ < TC_DATA_SIZE)
	{
		if (*p1++ != *p2++)
			return false;
	}
	return true;
}

IoEEPromClass IoEEProm;

