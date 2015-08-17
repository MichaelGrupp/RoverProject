/*
Interface for HC-SR04 measuring distances with ultrasound sensor

17.07.2013 Michael Grupp
*/

#include "Arduino.h"


#ifndef _USRANGING_
#define _USRANGING_


enum class Unit {
	METERS, CENTIMETERS, MILLIMETERS
};

typedef struct UltrasonicMeasurement {
	Unit unit;
	unsigned long duration;	//microseconds
	unsigned long distance;
};

class Ultrasonic
{
private:
	//Pins
	uint8_t pinTrigger;
	uint8_t pinEcho;

	UltrasonicMeasurement currentMeasurement;

public:
	//Constructors
	Ultrasonic(uint8_t pinTrigger, uint8_t pinEcho);
	Ultrasonic(uint8_t pinTrigger, uint8_t pinEcho, Unit unit);

	//setters
	void setUnit(Unit unit);

	//getters
	unsigned long getDistance();
	unsigned long getDuration();
	UltrasonicMeasurement getUltrasonicMeasurement();

	//measurement
	void measure();

};

#endif;