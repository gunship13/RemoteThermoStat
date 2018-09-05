// File: TemperatureSensor.cpp
// By: Mike Cipolla
// Date: 10/09/2015
#ifndef __PRESURE
	#include "TemperatureSensor.h"
#endif


void TemperatureSensorClass::init()
{


}

void TemperatureSensorClass::setInputPin(uint8_t pin)
{
	inputPin = pin;
}

void TemperatureSensorClass::readTempSensor()
{
	//static float tempSmooth;
	int tempRead;
	float tempReadF;
	//Note: It is best to USE 1.1 Volt Reference if possible since LM35 Output is < 1.0 V	
	//      The Mega Board with the LCD Touch needs the input to stay with 5V internal default
	//      for the 'touch' to work.
	// Not set here // analogReference(INTERNAL1V1); // This would be the command for Arduino Mega
	tempRead = analogRead(inputPin);  // Read in voltage from pin A8
	tempRead += 3;  // A/D Callibration with DVM measurment from testing on 8/4/2018

	tempReadC = ANALOG_LSB  * (float)tempRead;
	tempReadC /= TEMP_IN_CONVERT; // Temp in Celcius
	  
	
	rawTemp = tempReadC * 9.0 / 5.0 + 32;     // convert from deg. C to deg. F
	rawTemp += -1.8;    // Calibrate Sensor Value from testing on 8/4/2018

	// filterTemperature();
	if (twoSecondStartupFlag == true)
		smoothTemp = rawTemp;  // Kick start the smooth temperature.
	smoothTemp = smoothTemp * 0.9375 + rawTemp * 0.0625;  // filter to smooth the temperature reading.
}

char* TemperatureSensorClass::getTempTxt()
{
	static char outMsg[60];

	int wholSmTemp = (int)(smoothTemp);
	int fracSmTemp = (int)((smoothTemp - wholSmTemp) * 10);
	//sprintf(outMsg, "%d.%d F", wholSmTemp, fracSmTemp);
	int wholCTemp = (int)(tempReadC);
	int fracCTemp = (int)((tempReadC - wholCTemp) * 10);
	sprintf(outMsg, " %d.%d F %d.%d C ", wholSmTemp, fracSmTemp, wholCTemp, fracCTemp);
	return outMsg;
} 

float TemperatureSensorClass::getRoomTemp()
{
	return smoothTemp;
}

//  TemperatureSensorClass TemperatureSensor;

