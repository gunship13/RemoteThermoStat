// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       RmtThermostat.ino
    Created:	8/21/2018 9:36:55 PM
    Author:     LENARD\Mike
*/


#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

#include "ioSerialCommands.h"
#include "ioEther.h"
#include "ioEEProm.h"
#include "Relay.h"
#include "TStat.h"
#include "TouchButton.h"
#include "TxBox.h"
#include "TemperatureSensor.h"
#include "thTimer.h"
#include "TemperatureControl.h"
#include "Display.h"

#include "IoEEProm.h"

#define DEBUG

#ifdef DEBUG
char myMsg[80];
#endif

/*
************************************************************************************************************
*/
// Global Data
// Timers
// Caution:  Be sure to add any new clock timers to the initializeTimers() rotine so that clock rollover is prevented.
unsigned long previousClk_ms = 0L;
unsigned long previousClk125_ms = 0L;
unsigned long previousClk250_ms = 0L;
unsigned long previousOneSecClk_ms = 0L;
unsigned long previousTwoSecClk_ms = 0L;
unsigned long previousTempClk_ms = 0L;

// Define several timer durrations scaled to mili-seconds
const unsigned long time125_ms = 125L;
const unsigned long time250_ms = 250L;
const unsigned long timeOneSec = 1000L;
const unsigned long timeTwoSec = 2000L;
// 30 days is a time a little short of 50 days when the long timer will overflow
const unsigned long time30Days = 30 * 24 * 60 * 60 * 1000;

boolean twoSecondStartupFlag;
TemperatureSensorClass myTSenor;
TemperatureControl tempControl;
ThTimer theTime;
IoSerialCommandsClass IoSerialCommands;

// Ethernet Items Are Here
// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// Set the static IP address for your board
IPAddress ip(192, 168, 1, 50);

// Initialize the Ethernet client
EthernetServer server(80);

// Buffers for EEProm Data
char CopyTcEEProm[255];  // Need size of at least TC_DATA_SIZE
char NewTcEEProm[255];

// External Delclarations
extern volatile unsigned long timer0_millis;   // Get direct access to the Arduino timer used by millis() so
											   //   a graceful reset can be performed before it rolls over.

/*
************************************************************************************************************
*/

void setup()
{
#ifdef DEBUG
	//	Serial.begin(9600);            //  This is for Debug
	//	Serial.begin(115200);            //  This is for Debug
	//Serial.print("Startup Now.");
	//Serial.println();
#endif

	/*
	** Set up Ethernet
	*/

	// Start the Ethernet connection
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		Ethernet.begin(mac, ip);
	}

	// Start server
	server.begin();
	Serial.print("Server is at ");
	Serial.println(Ethernet.localIP());

	/*
	** Configure the arduino analog input A/D hardware
	*/
	if (REFERANCE_VOLTAGE == 1.1)
		analogReference(INTERNAL1V1);  // USE 1.1 Volt Reference since LM35 Output is < 1.0 V
	else if (REFERANCE_VOLTAGE != 5.0)
		analogReference(EXTERNAL);
	else
		analogReference(DEFAULT);

	myTSenor.setInputPin(A8);	// Pin A8 Has the input from the LM35 Analog Temperature Sensor
								// Note:  This sensor input needs to have a pull down resistor 
								// (1k Ohm seems to work well)

	theTime.initializeTimers();   // Set up all the timers

	IoEEProm.readEE2Local((char*)&localEECopy);
	tempControl.restoreTcData((char*)&localEECopy);

	IoSerialCommands.init();


	twoSecondStartupFlag = true;

	Display.init();
	Relay.init();

#ifdef DEBUG
	Serial.print("Startup is Done.");
	Serial.println();
#endif

}

/*
***************************************** MAIN LOOP ************************************************************
*/
void loop()
{
	char myRmTemp[40];
	unsigned long currentClk_ms = millis();


	/*                 // The 125ms Code Block //
	************************************************************************************************************
	*/
	if (currentClk_ms - previousClk_ms > time125_ms) {

		previousClk_ms = currentClk_ms;

	}  // end of 125 ms block


	   /*                 // The 250ms Code Block //
	   ************************************************************************************************************
	   */
	if (currentClk_ms - previousClk250_ms > time250_ms) {
		previousClk250_ms = currentClk_ms;
		String newCmd = "CCC";
		newCmd = IoSerialCommands.readPortString();
		// Serial.print(newCmd);
		IoSerialCommands.parseCommand(newCmd);

		myTSenor.readTempSensor();  // Read in, filter, and convert room temperature to degrees F

		Display.processButtons();
		Display.write();
	}


	/*                 // One Second Code Block  //
	************************************************************************************************************
	*/
	currentClk_ms = millis();

	if (currentClk_ms - previousOneSecClk_ms > timeOneSec) {
		previousOneSecClk_ms = currentClk_ms;

		if (millis() > time30Days)            // If we are getting close to timer overflow, then reset all timers
			theTime.initializeTimers();

	}  // end One Second Block


	   /*                 // Two Second Code Block  //
	   ************************************************************************************************************
	   */
	currentClk_ms = millis();

	if (currentClk_ms - previousTwoSecClk_ms > timeTwoSec) {
		previousTwoSecClk_ms = currentClk_ms;
		twoSecondStartupFlag = false;

		sprintf(myRmTemp, "Room Temp:%s", myTSenor.getTempTxt());
		TxBox(20, 10, 220, 22, YELLOW, BLUE, myRmTemp);
		u8 runCmd = tempControl.runControlRoomTemp();
		Relay.commandRelays(runCmd);

		// Keep EEProm Up To Date
		tempControl.copyTcData((char*)&localTcData);
		IoEEProm.readEE2Local((char*)&localEECopy);
		boolean EEPromUptodate = IoEEProm.areBuffsSame((char*)&localTcData, (char*)&localEECopy);
		if (!EEPromUptodate)
			IoEEProm.writeLocal2EE((char*)&localTcData);

		/**
		** <<<<<<<<<<<< The Ethernet Code follows  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		*/

		// Measure the humidity & temperature
	//	float h = 13.34; // dht.readHumidity();
	//	float t = 77.499;  // dht.readTemperature();

						  // Transform to String
	//	String temp = String((int)(t+0.5));
	//	String hum = String((int)(h+0.5));

		// Listen for incoming clients
		EthernetClient client = server.available();
		if (client) {

			Serial.println("New client");

			// An HTTP request ends with a blank line
			boolean currentLineIsBlank = true;
			while (client.connected()) {

				// Read data
				if (client.available()) {
					char c = client.read();
					Serial.write(c);

					// Send a reply if end of line detected
					if (c == '\n' && currentLineIsBlank) {

						// Send a standard HTTP response header
						client.println("HTTP/1.1 200 OK");
						client.println("Content-Type: text/html");
						client.println("Connection: close");
						client.println("Refresh: 5");  // Refresh the page automatically every 5 sec
						client.println();
						client.println("<!DOCTYPE HTML>");
						client.println("<html>");

						client.println("Arduino Thermostat Status");
						client.print("<br />");
						client.print("<br />");
						// Output the value of the temperature  
						client.print("Room Temperature: ");
						client.print(myTSenor.getTempTxt());
						client.print("<br />");
						client.print("Mode: ");
						client.print(tempControl.getSysModeTx());
						client.println("<br />");
						client.println("</html>");
						break;
					}
					if (c == '\n') {
						// you're starting a new line
						currentLineIsBlank = true;
					}
					else if (c != '\r') {
						// you've gotten a character on the current line
						currentLineIsBlank = false;
					}
				}
			}
			// Give the web browser time to receive the data
			delay(1);
			// Close the connection:
			client.stop();
			Serial.println("Client disconnected");
		}
		/*
		
		**  <<<<<<<<<<< End of Ethernet code  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		*/
#ifdef SKIP
		Serial.print(currentClk_ms / 1000); Serial.print(" tmpCmd = "); Serial.print(runCmd); Serial.print(myRmTemp);
		//		Serial.print(" EE="); Serial.print(EEPromUptodate);
		Serial.println();
#endif

	}  // end 2 second block

}  // End of MAIN LOOP
   // EOF : RTStat.ino
