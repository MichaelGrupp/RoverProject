/*
Interface for 10-segment Grove LED bar

Display functions for showing values from 0-1024:
-10-bit counter
-linear mapping (stack)

16.07.2013 Michael Grupp, revision: 16.08.2015 
*/

#include "Arduino.h"


#ifndef _LEDBAR_
#define _LEDBAR_

class LEDBar
{
private:
	//Pins
	uint8_t pinData;		//yellow cable
	uint8_t pinClock;		//white cable

	//mapping values
	int mapLow;
	int mapHigh;
	int mapped;

	uint16_t output;

	//LED control
	uint8_t
		clkpin, datapin,		//Clock & data pin numbers
		clkpinmask, datapinmask;//Clock & data PORT bitmasks
	volatile uint8_t
		*clkport, *dataport;	//Clock & data PORT registers

public:
	//Constructors
	LEDBar(uint8_t pinData, uint8_t pinClock);
	LEDBar(uint8_t pinData, uint8_t pinClock, int mapLow, int mapHigh);

	//setters
	void setMapLow(int mapLow);
	void setMapHigh(int mapHigh);

	//getters
	uint8_t getPinData();
	uint8_t getPinClock();
	int getMapLow();
	int getMapHigh();

	//10-bit counter display
	void displayCounter(uint16_t displayVal);

	//Linear stack display - direction==0 grows stack from green to red
	void displayStack(uint16_t displayVal, bool direction);

	//LED-Ansteuerung
	void sendLED(uint16_t data);
	void setCmdMode();
	void latchData();
	void setSingleBar(uint8_t barnum);
	void send16bitData(uint16_t data);

};

#endif;