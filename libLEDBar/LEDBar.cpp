/*
Interface for 10-segment Grove LED bar

Display functions for showing values from 0-1024:
-10-bit counter
-linear mapping (stack)

16.07.2013 Michael Grupp, revision: 16.08.2015
*/

#include "LEDBar.hpp"


LEDBar::LEDBar(uint8_t pinData, uint8_t pinClock)
{
	this->pinData = pinData;
	this->pinClock = pinClock;
	pinMode(pinData, OUTPUT);
	pinMode(pinClock, OUTPUT);
	this->mapLow = 0;
	this->mapHigh = 1023;
	clkport = portOutputRegister(digitalPinToPort(pinClock));
	clkpinmask = digitalPinToBitMask(pinClock);
	dataport = portOutputRegister(digitalPinToPort(pinData));
	datapinmask = digitalPinToBitMask(pinData);
}
LEDBar::LEDBar(uint8_t pinData, uint8_t pinClock, int mapLow, int mapHigh)
{
	this->pinData = pinData;
	this->pinClock = pinClock;
	pinMode(pinData, OUTPUT);
	pinMode(pinClock, OUTPUT);
	this->mapLow = mapLow;
	this->mapHigh = mapHigh;
	clkport = portOutputRegister(digitalPinToPort(pinClock));
	clkpinmask = digitalPinToBitMask(pinClock);
	dataport = portOutputRegister(digitalPinToPort(pinData));
	datapinmask = digitalPinToBitMask(pinData);
}


void LEDBar::setMapLow(int mapLow)
{
	this->mapLow = mapLow;
}
void LEDBar::setMapHigh(int mapHigh)
{
	this->mapHigh = mapHigh;
}


uint8_t LEDBar::getPinData()
{
	return this->pinData;
}
uint8_t LEDBar::getPinClock()
{
	return this->pinClock;
}
int LEDBar::getMapLow()
{
	return this->mapLow;
}
int LEDBar::getMapHigh()
{
	return this->mapHigh;
}


void LEDBar::displayCounter(uint16_t displayVal)
{
	setCmdMode();
	sendLED(displayVal);
	latchData();
}

void LEDBar::displayStack(uint16_t displayVal, bool direction)
{
	mapped = map(displayVal, mapLow, mapHigh, 0, 10);
	if (direction == 0) {
		switch (mapped) {
		case 0: output = 0; break;
		case 1: output = 512; break;
		case 2: output = 768; break;
		case 3: output = 896; break;
		case 4: output = 960; break;
		case 5: output = 992; break;	//green to red
		case 6: output = 1008; break;
		case 7: output = 1016; break;
		case 8: output = 1020; break;
		case 9: output = 1022; break;
		case 10: output = 1023; break;
		}
	}
	else {
		switch (mapped) {
		case 0: output = 0; break;
		case 1: output = 1; break;
		case 2: output = 3; break;
		case 3: output = 7; break;
		case 4: output = 15; break;
		case 5: output = 31; break;		//red to green
		case 6: output = 63; break;
		case 7: output = 127; break;
		case 8: output = 255; break;
		case 9: output = 511; break;
		case 10: output = 1023; break;
		}
	}
	setCmdMode();
	sendLED(output);
	latchData();
}

void LEDBar::sendLED(uint16_t data)
{
	unsigned char i;
	for (i = 0; i<12; i++) {
		if (data & 0x0001)
			send16bitData(0x00ff);
		else
			send16bitData(0x0000);
		data = data >> 1;
	}
}

void LEDBar::setSingleBar(uint8_t barnum)
{
	setCmdMode();
	sendLED(0x01 << barnum);
	latchData();
}

void LEDBar::setCmdMode(void)
{
	send16bitData(0x0000);
}
void LEDBar::send16bitData(uint16_t data)
{
	for (unsigned char i = 0; i<16; i++) {
		if (data & 0x8000) {
			*dataport |= datapinmask;
		}
		else {
			*dataport &= ~datapinmask;
		}
		*clkport ^= clkpinmask;
		data <<= 1;
	}
}

void LEDBar::latchData(void)
{
	*clkport &= ~datapinmask;
	delayMicroseconds(10);
	for (unsigned char i = 0; i<8; i++) {
		*clkport ^= datapinmask;
	}
}
