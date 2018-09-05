// File:  runTemperatueControl.cpp
// By:  Mike Cipolla
// Data:  06/07/2015
// 
// 
// 

#include "TemperatureControl.h"
#include "TemperatureSensor.h"
#include "TStat.h"

char localTcData[TC_DATA_SIZE];
char localEECopy[TC_DATA_SIZE];

extern TemperatureSensorClass myTSenor;

void TemperatureControl::init()
{


}

TemperatureControl::TemperatureControl(void)  // Constructor
{


}
/*
**  Place a copy of the TemperatureControl data into prescribed memory
*/
u8 TemperatureControl::copyTcData(char* writeBuff)
{
	u8 chCount = TC_DATA_SIZE;
	char* chBuff = (char*) &startWord;  // This is the start of data in TC Data Buffer
#ifdef SKIP
	Serial.print("*chBuff =");
	Serial.println();
#endif
	while (chCount--)
		//		*writeBuff++ = *chBuff++;
	{

		*writeBuff++ = *chBuff++;
#ifdef SKIP
		Serial.print((u8)(*chBuff-1)); Serial.print(",");  // -1 is to compensate for the *chBuff++ above
#endif
	}
	return TC_DATA_SIZE;
}

u8 TemperatureControl::restoreTcData(char* readBuff)
{
	u8 chCount = TC_DATA_SIZE;
	char* chBuff = (char*)&startWord;  // This is the start of data in TC Data Buffer
	while (chCount--)
		*chBuff++ = *readBuff++;
	// Do some critical validity checks and clean out any bad data
	//    We use the set commands since they do error checking
	tempControl.setHighTemp(highTemp);
	tempControl.setLowTemp( lowTemp);
	tempControl.setSysMode(sysMode);
	// Fan doesn't have any validity check since it is boolean without a range value.

#ifdef DEBUG
	Serial.print("Exit of restoreTcData.");
	Serial.println();
#endif
	
	return TC_DATA_SIZE;
}



//  This module performs the Main Thermostat Functions of monitoring and contolling of the room temperature
void TemperatureControl::setSysMode(int modeSet)
{
	if ((modeSet <= MAX_MODE_NUMBER) && modeSet >= 0)
		sysMode = modeSet;
	else
		sysMode = SYS_OFF;

	// Set up Start and End of Data to standard Values
}

int TemperatureControl::getSysMode()
{
	return sysMode;
}

int TemperatureControl::getRunCommand()
{
	return runCommand;
}
void TemperatureControl::setSysFanOn()
{
	sysFan = true;
}

void TemperatureControl::toggleSysFan()
{
	if (sysFan == true)
		sysFan = false;
	else
		sysFan = true;
}

void TemperatureControl::setSysFanOff()
{
	sysFan = false;
}

bool TemperatureControl::getSysFanOn()
{
	return sysFan;   // True means fan is commanded on
}

 char sysModeChar[9];

char* TemperatureControl::getSysModeTx()
{
	switch (sysMode){
	case SYS_HEAT:
		sprintf(sysModeChar, "HEAT");
		break;
	case SYS_COOL:
		sprintf(sysModeChar, "COOL");
		break;
	case SYS_AUTO:
		sprintf(sysModeChar, "AUTO");
		break;
	case SYS_OFF:
		sprintf(sysModeChar, "OFF");
		break;
	case SYS_EMR_HEAT:
		sprintf(sysModeChar, "EMR HEAT");
		break;
	default :
		sprintf(sysModeChar, "Invalid Mode");
	}

	return sysModeChar;

}
/*
**********************************************************************************************
*  Function: getSysFanTx()
*/
char sysFanChar[9];

char* TemperatureControl::getSysFanTx()
{
	if (sysFan)
		sprintf(sysFanChar, " FAN ON");
	else
		sprintf(sysFanChar, "");
	return sysFanChar;
}

/*
**********************************************************************************************
*  Function: runThermostat()
*/
u8 TemperatureControl::runControlRoomTemp(void)
{

	float rmTemp = myTSenor.getRoomTemp();   //TemperatureSensorClass myTSenor;

	switch (sysMode){

	case SYS_COOL:
		if (rmTemp > (highTemp + TEMP_DIFF))
			runCommand = CMD_COOL;
		else if (rmTemp < (highTemp - TEMP_DIFF))
			runCommand = CMD_IDLE;
		break;

	case SYS_HEAT:
		if (rmTemp >(lowTemp + TEMP_DIFF))
			runCommand = CMD_IDLE;
		else if (rmTemp < (lowTemp - TEMP_EM_DIFF))  // IF way behind in heating room
			runCommand = CMD_EMR_HEAT;
		// Once EM Heat is on, keep it on for at least a little while. 
		else if ((runCommand == CMD_EMR_HEAT) && (rmTemp < (lowTemp - TEMP_EM_DIFF - 0.2)))
			runCommand = CMD_EMR_HEAT;
		else if (rmTemp < (lowTemp - TEMP_DIFF)) 
			runCommand = CMD_HEAT;               
		break;

	case SYS_EMR_HEAT:
		if (rmTemp >(lowTemp + TEMP_DIFF))
			runCommand = CMD_IDLE;
		else if (rmTemp < (lowTemp - TEMP_DIFF))
			runCommand = CMD_EMR_HEAT;
		break;

	case SYS_AUTO:
		if ((rmTemp >(lowTemp + TEMP_DIFF)) && (rmTemp < (highTemp - TEMP_DIFF)))
			runCommand = CMD_IDLE;
		else if (rmTemp < (lowTemp - TEMP_EM_DIFF))  // IF way behind in heating the room
			runCommand = CMD_EMR_HEAT;
		// Once EM Heat is on, keep it on for at least a little while. 
		else if ((runCommand == CMD_EMR_HEAT) && (rmTemp < (lowTemp - TEMP_EM_DIFF - 0.2)))
			runCommand = CMD_EMR_HEAT;
		else if ((rmTemp < (lowTemp - TEMP_DIFF)))
			runCommand = CMD_HEAT;
		else if (rmTemp >(highTemp + TEMP_DIFF))
			runCommand = CMD_COOL;
		break;


	default:
	case SYS_OFF:
		runCommand = CMD_IDLE;
		break;
	} // end switch  

	return runCommand;

}  // end runThermostat()

void TemperatureControl::setHighTemp(int hTemp)
{
	if (hTemp > MAX_TEMP_CMD) 
		highTemp = MAX_TEMP_CMD;
	else if (hTemp > (MIN_TEMP_CMD + MIN_TEMP_DIF))
		highTemp = hTemp;
	else
		highTemp = MIN_TEMP_CMD + MIN_TEMP_DIF;
		
	// Adjust lowTemp if needed to keep it less than highTemp-MIN_TEMP_DIF
	if (lowTemp > (highTemp - MIN_TEMP_DIF))
		lowTemp = highTemp - MIN_TEMP_DIF;
}

int TemperatureControl::incrementHighTemp()
{
	if (highTemp < MAX_TEMP_CMD)
		highTemp++;

	return highTemp;
}

int TemperatureControl::decrementHighTemp()
{
	if (highTemp > (MIN_TEMP_CMD + MIN_TEMP_DIF))
		highTemp--;

	// Adjust lowTemp if needed to keep it less than highTemp-MIN_TEMP_DIF
	if (lowTemp > (highTemp - MIN_TEMP_DIF))
		lowTemp = highTemp - MIN_TEMP_DIF;

	return highTemp;
}

int TemperatureControl::getHighTemp(void)
{
	return highTemp;
}

int TemperatureControl::incrementLowTemp()
{
	if (lowTemp < (MAX_TEMP_CMD-MIN_TEMP_DIF))
		lowTemp++;

	// Adjust highTemp if needed to keep it more than lowTemp+MIN_TEMP_DIF
	if (highTemp < (lowTemp + MIN_TEMP_DIF))
		highTemp = lowTemp + MIN_TEMP_DIF;

	return lowTemp;
}

int TemperatureControl::decrementLowTemp()
{
	if (lowTemp > MIN_TEMP_CMD)
		lowTemp--;
	return lowTemp;
}

void TemperatureControl::setLowTemp(int lTemp)
{
	if (lTemp < MIN_TEMP_CMD)
		lowTemp = MIN_TEMP_CMD;
	else if (lTemp < (MAX_TEMP_CMD - MIN_TEMP_DIF))
		lowTemp = lTemp;
	else
		lowTemp = MAX_TEMP_CMD - MIN_TEMP_DIF;

	// Adjust highTemp if needed to keep it more than lowTemp+MIN_TEMP_DIF
	if (highTemp < (lowTemp + MIN_TEMP_DIF))
		highTemp = lowTemp + MIN_TEMP_DIF;
}

int TemperatureControl::getLowTemp(void)
{
	return lowTemp;
}
void TemperatureControl::setTempLowHi(int lTemp, int hTemp)
{
	setHighTemp(hTemp);	// Note that setLowTemp() may modify the highTemp to keep highTemp > (minTemp + MIN_TEMP_DIF)
	setLowTemp(lTemp);  // So set setLowTemp takes priority since it is called last.
}

//RunTemperatureControlClass TemperatureControl;

