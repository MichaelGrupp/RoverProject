#include "ultrasonicRanging.hpp"

Ultrasonic::Ultrasonic(uint8_t pinTrigger, uint8_t pinEcho)
{
	this->pinTrigger = pinTrigger;
	this->pinEcho = pinEcho;
	pinMode(pinTrigger, OUTPUT);
	pinMode(pinEcho, INPUT);
	currentMeasurement.unit = Unit::CENTIMETERS;
	currentMeasurement.distance = 0;
	currentMeasurement.duration = 0;
}

Ultrasonic::Ultrasonic(uint8_t pinTrigger, uint8_t pinEcho, Unit unit)
{
	this->pinTrigger = pinTrigger;
	this->pinEcho = pinEcho;
	pinMode(pinTrigger, OUTPUT);
	pinMode(pinEcho, INPUT);
	currentMeasurement.unit = unit;
	currentMeasurement.distance = 0;
	currentMeasurement.duration = 0;
}

void Ultrasonic::setUnit(Unit unit)
{
	currentMeasurement.unit = unit;
}

unsigned long Ultrasonic::getDistance()
{
	return currentMeasurement.distance;
}

unsigned long Ultrasonic::getDuration()
{
	return currentMeasurement.duration;
}

UltrasonicMeasurement Ultrasonic::getUltrasonicMeasurement()
{
	return currentMeasurement;
}

void Ultrasonic::measure()
{
	digitalWrite(pinTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(pinTrigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(pinTrigger, LOW);
	currentMeasurement.duration = pulseIn(pinEcho, HIGH);	//in microseconds
	if (currentMeasurement.unit == Unit::CENTIMETERS) {
		currentMeasurement.distance = (currentMeasurement.duration / 2) / 29.15;
	}
	else if (currentMeasurement.unit == Unit::METERS) {
		currentMeasurement.distance = (currentMeasurement.duration / 2) / 2915;
	}
	else if (currentMeasurement.unit == Unit::MILLIMETERS) {
		currentMeasurement.distance = (currentMeasurement.duration / 2) / 0.2915;
	}
}

bool Ultrasonic::inRange()
{
	if (currentMeasurement.unit == Unit::CENTIMETERS) {
		if (currentMeasurement.distance < 300 && currentMeasurement.distance >= 0)
			return true;
		else
			return false;
	}
	else if (currentMeasurement.unit == Unit::METERS) {
		if (currentMeasurement.distance < 3 && currentMeasurement.distance >= 0)
			return true;
		else 
			return false;
	}
	else if (currentMeasurement.unit == Unit::MILLIMETERS) {
		if (currentMeasurement.distance < 3000 && currentMeasurement.distance >= 0)
			return true;
		else
			return false;
	}
}
