// TxBox.h

#ifndef _TXBOX_h
#define _TXBOX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include <TFTv2.h>
#else
	#include "WProgram.h"
#endif

class TxBox
{
	INT16U xp;
	INT16U yp;
	INT16U xw;
	INT16U yw;
	INT16U chColor;
	INT16U bkColor;
	char   *txt;
	INT16U size;
	TFT    cWin;
	
 protected:


 public:
	void init();
	TxBox(INT16U xp, INT16U yp, INT16U xw, INT16U yw, INT16U chColor, INT16U bkColor, char *txt);

};


//extern TxBoxClass TxBox;

#endif

