// 
// 
// 

#include "TxBox.h"

void TxBox::init()
{


}

 TxBox::TxBox(INT16U ixp, INT16U iyp, INT16U ixw, INT16U iyw, INT16U ichColor, INT16U ibkColor, char *itxt){
	xp = ixp; yp = iyp; xw = ixw; yw = iyw; chColor = ichColor; bkColor = ibkColor; txt = itxt;
	size = max(2, (yw / 10));
	//cWin.TFTinit();
	Tft.fillRectangle(xp, yp, xw, yw, bkColor);
	Tft.drawString(txt, xp + 1, yp + 4, size, chColor);
	//  cWin.drawNumber(size, 10, 60, 2, GREEN);
}


// TxBox TxBox;

