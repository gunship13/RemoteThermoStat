// TemperatureControl.h

#ifndef _TEMPERATURECONTROL_h
#define _TEMPERATURECONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class TemperatureControl
{
 protected:
	 // Caution:  May need a compiler dirctive to keep these in the correct order
	 //  ??? I don't know what that is at the moment but we may need it later
	 u16 startWord;  // address of this is used as a pointer for status buffer copy
	 u8 runCommand;	// Commands sent to the Furnace
	 u8 sysMode;		// User CMD Mode: Off, Cool, Heat, Emegency Heat, Auto (Both Heat and Cool)
	 boolean sysFan;        // Indicates the Fan will be on full time
	 u8 highTemp;		// When Room gets above this, Cooling will turn on
	 u8 lowTemp;		// When Romm gets below this, Heat will turn on
	 u16 padding;       // Extra Bytes of padding

#define TC_DATA_SIZE sizeof(TemperatureControl)   // This will be used by all the buffering and copying 
	 // ??? Is this just the size of the data above or also the functions below?

public:
	void init();
	TemperatureControl(void);   // Constructor
	u8 copyTcData(char* toBuff);  // returns number of bytes
	u8 restoreTcData(char* fromBuff);  // returns number of bytes
	void setSysMode(int modeSet);
	int   getSysMode(void);
	int   getRunCommand(void);
	char* getSysModeTx(void);
	char* getSysFanTx(void);
	void setSysFanOff(void);
	void setSysFanOn(void);
	void toggleSysFan(void);
	bool getSysFanOn(void);
	u8 runControlRoomTemp(void);
	void setHighTemp(int hTemp);
	int  getHighTemp(void);
	int  incrementHighTemp(void);
	int  decrementHighTemp(void);
	void setLowTemp(int);
	int  getLowTemp(void);
	int  incrementLowTemp(void);
	int  decrementLowTemp(void);
	void setTempLowHi(int lTemp, int hTemp);

	
};

// extern TemperatureControl RunTemperatureControlDummy;
extern TemperatureControl tempControl;
extern char localTcData[TC_DATA_SIZE];
extern char localEECopy[TC_DATA_SIZE];

#endif

