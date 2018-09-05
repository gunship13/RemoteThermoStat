// File: ioEther.h
// By:  Mike Cipolla
// Date:  10/10/2015

#ifndef _IOETHER_h
#define _IOETHER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class IoEtherClass
{
 protected:


 public:
	void init();
};

extern IoEtherClass IoEther;

#endif

