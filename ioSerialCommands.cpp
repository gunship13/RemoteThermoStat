// File ioSerialCommands.cpp
// By: Mike Cipolla
// Date: 7/9/2018

#include "ioSerialCommands.h"
#include "TemperatureControl.h"
#include "TemperatureSensor.h"
#include "TStat.h"
#include "Relay.h"
void IoSerialCommandsClass::init()
{
	Serial.begin(9600);
	
	if (Serial.available() > 0) {
		Serial.println("IoSerialCommands has started");
		ioPortOpen = true;
	}

}

String IoSerialCommandsClass::readPortString()
{
	ioString[0] = 0;
	Serial.readBytesUntil(';', ioString, IO_LENGTH);
	return ioString;
}
void IoSerialCommandsClass::parseCommand(String str)
{
	int eqIndex  = 0;
	eqIndex = str.indexOf('=', 0);
	String cmdType = "";
	bool sendMsg = false;

	static char ioMsg[100];
	ioMsg[0] = 0;


	if (eqIndex >= 2) {
		cmdType = str.substring(eqIndex - 2, eqIndex);  //two chars before the equal sign
		cmdValue = str.substring(eqIndex + 1, eqIndex + 3);  //two chars after the equal sign
		sendMsg = true;
	}

	if (cmdType.equalsIgnoreCase("TH"))  // Set a Temperature High setting
	{
		int iTemp = cmdValue.toInt();
		tempControl.setHighTemp(iTemp);
		sprintf(ioMsg, " ++ Set High Temp to: %d\n", iTemp);

	}
	else if (cmdType.equalsIgnoreCase("TL"))  // Set a Temperature Low setting
	{
		int iTemp = cmdValue.toInt();
		tempControl.setLowTemp(iTemp);
		sprintf(ioMsg, " -- Set Low Temp to: %d\n", iTemp);
	
	}
	else if (cmdType.equalsIgnoreCase("MD"))  // Set system mode
	{
		int iMode = cmdValue.toInt();
		tempControl.setSysMode(iMode);
		sprintf(ioMsg, " ** Set Mode to: %s\n",tempControl.getSysModeTx());
		
	}
	else if (cmdType.equalsIgnoreCase("ST"))  // Print Theomostatus Status
	{
		sprintf(ioMsg, "Room Temp: %s Mode: %s HiTemp: %d LoTemp: %d\n",
			myTSenor.getTempTxt(), tempControl.getSysModeTx(), tempControl.getHighTemp(), tempControl.getLowTemp());
	}
	else if (cmdType.equalsIgnoreCase("RT"))  // Print Room Temperature
	{
		sprintf(ioMsg, "Room Temp:%s\n", myTSenor.getTempTxt());
	}
	else if (cmdType.equalsIgnoreCase("RL"))  // Print Relay Status
	{
		sprintf(ioMsg, " Relay Cmd = 0x%x\n", Relay.status());
	}
	if (sendMsg) {
		//Serial.print(">>> ");
		Serial.print(cmdType);
		Serial.print("> ");
		Serial.print(ioMsg);
	}
	
}



