// TemperatureSensor.h

#ifndef _TEMPERATURESENSOR_h
#define _TEMPERATURESENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include "TStat.h"
#else
	#include "WProgram.h"
#endif

class TemperatureSensorClass
{
 protected:
	  float smoothTemp;
	  float tempReadC;
	  float rawTemp;
	 uint8_t inputPin;


 public:
	char tempTxt[7];	
	
	void init();
	void setInputPin(uint8_t);   // Connects the Temp Sensor (Part LM-35) to Requested Analog Input Pin
	void readTempSensor();       // Commands AD Conversion of input, convets to deg Feirenheit, and smooths the reading
	char* getTempTxt();          // Gets the smoothed temperature formated as "88.8 F"
	float getRoomTemp();         // Gets the room temperature as a float number

};

 extern TemperatureSensorClass myTSenor;

#endif

