// NOTE:  The relay board does not work with SERIAL I/O So Do Not Set DEBUG if connected to Furnace

#ifndef TH_DEFINE

#define TH_DEFINE
#define DEBUG          // Comment this line out if you do not want to include Debug Code
#define VERSION  0        // This is minor version displayed at startup
#define TOP_LINE 0
#define BOT_LINE 1
#define COL_ONE  0

#define CMD_IDLE      0   //  Mode Commands Output to Relays by Thermostat
#define CMD_COOL      1
#define CMD_HEAT      2
#define CMD_EMR_HEAT  3
#define CMD_FAN_ONLY  4
//#define UNO  1
//#define MEGA 2 // look at this instead:  __AVR_ATmega2560__
#define BOARD_MEGA


#define MIN_INPUT false              // Input State for Min Input Temp with Auto Mode
#define MAX_INPUT true               // Input State for Max Input Temp with Auto Mode

// Define Constants for EEPROM and API interface
#define HEADER_SYNC      0xA5A55A5A
#define PROTOCAL_VERSION     0x0001
#define THERMOSTAT_CMD       0x00F0

#define LCD_HW_V0                // This would be for unmodified HW
//#define LCD_HW_V1                  // use Analog Reverence Voltate In Pin 0

#ifdef LCD_HW_V0
	#define REFERANCE_VOLTAGE 5.0
#endif


#ifdef LCD_HW_V1
#define REFERANCE_VOLTAGE 1.879
#endif

#define ANALOG_LSB (REFERANCE_VOLTAGE/1024.0)        // define LSB for 10 bit A/D conversion

#define TEMP_IN_CONVERT 0.010  // 10mV per Deg Celsius
#define TEMP_DIFF 0.4          // This provides the hysteresis for the Heating and Cooling Cycling On and Off
#define TEMP_EM_DIFF 2        // In HEAT Mode, when Temperatre Lags by this amount then kick in Emergency Heat
// Mode menu Items

#define MIN_TEMP_CMD     40         // This program will limit the Temp Cmd to min of 40 deg Fahrenheit
#define MAX_TEMP_CMD     99         // This program will limit the Temp Cmd to max of 99 deg Fahrenheit 
#define MIN_TEMP_DIF      4         // Keep tempHi and tempLow more than 4 degrees apart

#define KEY_HOLD_HALF_SEC 2         // 250ms * 2 = Half Second 

enum SYS_MODES { SYS_OFF, SYS_COOL, SYS_HEAT, SYS_EMR_HEAT, SYS_AUTO };
#define MAX_MODE_NUMBER   SYS_AUTO
// Note Keep SYS_AUTO as the last item since it is used as the Max Index for some error checking

// Define Structure To Hold Values in EEProm Memory across loss of power or restarts
// This will also be used in the future as an API Interface for remote control of the thermostat.
typedef struct TflashSave {
	unsigned long  headerSync;           // 0xA5A55A5A
	int            msgLength;            // This length tells how many bytes follow. (does not include headerSync or msgLength)
	int            protocallVersion;     // 0x0001 is the first version
	int            msgType;              // 0x00F0 is the only message for now
	enum SYS_MODES menuMode;             // Index to the current menu being displayed
	enum SYS_MODES bufferedMode;         // Tentative Mode Command
	enum SYS_MODES operateMode;          // Final Active Mode Command
	enum SYS_MODES returnFromAutoMode;   // Hold the mode AUTO MODE was entered from
	int            minAutoTemp;          // Minimum Temp Command for Auto Mode (Where Heat is switched on)
	int            maxAutoTemp;          // Maximum Temp Command for Auto Mode (Where Cool is switched on)
	//  Compiler did not like this: boolean        sysFanOn;             
	boolean        sysFanOn;             // False - set the fan off except when another mode needs it.  Often called "Fan Auto".

	//   So the "Fan Always On" would be indicated with this flag set "true"
	int            tempCmd[MAX_MODE_NUMBER]; // The user commanded temperature for each mode
	int            spare_FFFF;           // Set this to 0xFFFF to help with debug
};
extern boolean twoSecondStartupFlag;
//extern TouchButton *p_tb1;
//extern TemperatureSensorClass myTSenor;
//extern ThTimer theTime;
#endif  // TH_DEFINE
