// SystemState.h

#ifndef _SYSTEMSTATE_h
#define _SYSTEMSTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SystemStateClass
{
 protected:


 public:
	void init();
	int BrewingState;
	double HotWaterLiquidTankTemperature;
};

extern SystemStateClass SystemState;

#endif

