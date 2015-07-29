/*
Klasse für LED-Bar zum Einsatz mit Romeo V2.0 im letsgoING-Fahrzeug
Funktionen: Analogwert (0-1024) umwandeln (mappen) und passenden Balken anzeigen, Werte z.B. von Distanzsensor
Teile der LED-Ansteuerung aus github.com/sysrun/GroveLedBar
(c)16.07.2013 Michael Grupp
*/

#include "LEDBar.hpp"

//Konstruktoren
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

//setter (Pins hier nicht sinnvoll)
void LEDBar::setMapLow(int mapLow)
{
	//if (0<=mapLow<=1023 && mapLow<this->mapHigh)
	this->mapLow = mapLow;
	//else
	//this->mapLow=0;
}
void LEDBar::setMapHigh(int mapHigh)
{
	//if (0<=mapHigh<=1023 && mapHigh>this->mapLow)
	this->mapHigh = mapHigh;
	//else
	//this->mapHigh=1023;
}


//getter
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

//Anzeige als 10bit-Zähler (Lücken)
void LEDBar::analogToCounter(uint16_t analogVal)
{
	setCmdMode();
	sendLED(analogVal);
	latchData();
}

//Anzeige als Stack	(Lückenlos)
//direction=0 -> von grün nach rot  direction=1 -> von rot nach grün
void LEDBar::analogToStack(uint16_t analogVal, bool direction)
{
	mapped = map(analogVal, mapLow, mapHigh, 0, 10);
	if (direction == 0) {
		switch (mapped) {
		case 0: output = 0; break;
		case 1: output = 512; break;
		case 2: output = 768; break;
		case 3: output = 896; break;
		case 4: output = 960; break;
		case 5: output = 992; break;		 //grün nach rot
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
		case 5: output = 31; break;		//rot nach grün
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

//LED-Ansteuerung
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

//Latch Routine
void LEDBar::latchData(void)
{
	*clkport &= ~datapinmask;
	delayMicroseconds(10);
	for (unsigned char i = 0; i<8; i++) {
		*clkport ^= datapinmask;
	}
}
