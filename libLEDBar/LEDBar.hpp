/*
Klasse für LED-Bar zum Einsatz mit Romeo V2.0 im letsgoING-Fahrzeug
Funktionen: Analogwert (0-1023) anzeigen, z.B. von Distanzsensor
Teile zur LED-Ansteuerung (GroveLedbar.h) aus github.com/sysrun/GroveLedBar
(c)16.07.2013 Michael Grupp
*/

#include "Arduino.h"


#ifndef _LGILEDBAR_
#define _LGILEDBAR_

class LEDBar
{
private:
	//Pins
	uint8_t pinData;		//gelbes Kabel
	uint8_t pinClock;		//weißes Kabel

	//mapping-Grenzen (min-max-Analogeingang)
	int mapLow;
	int mapHigh;
	int mapped;

	uint16_t output;

	//für LED-Ansteuerung
	uint8_t
		clkpin, datapin,     // Clock & data pin numbers
		clkpinmask, datapinmask; // Clock & data PORT bitmasks
	volatile uint8_t
		*clkport, *dataport;   // Clock & data PORT registers

public:
	//Konstruktoren
	LEDBar(uint8_t pinData, uint8_t pinClock);
	LEDBar(uint8_t pinData, uint8_t pinClock, int mapLow, int mapHigh);

	//setter (Pins hier nicht sinnvoll)
	void setMapLow(int mapLow);
	void setMapHigh(int mapHigh);

	//getter
	uint8_t getPinData();
	uint8_t getPinClock();
	int getMapLow();
	int getMapHigh();

	//Anzeige als 10bit-Zähler (Lücken)
	void analogToCounter(uint16_t analogVal);

	//Anzeige als Stack	(Lückenlos)
	void analogToStack(uint16_t analogVal, bool direction);

	//LED-Ansteuerung
	void sendLED(uint16_t data);
	void setCmdMode();
	void latchData();
	void setSingleBar(uint8_t barnum);
	void send16bitData(uint16_t data);

};

#endif;