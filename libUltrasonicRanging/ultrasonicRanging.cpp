#include "ultrasonicRanging.hpp"

Ultrasonic::Ultrasonic(uint8_t pinTrigger, uint8_t pinEcho)
{
	this->pinTrigger = pinTrigger;
	this->pinEcho = pinEcho;
	currentMeasurement.unit = Unit::CENTIMETERS;
	currentMeasurement.distance = 0;
	currentMeasurement.duration = 0;
}

Ultrasonic::Ultrasonic(uint8_t pinTrigger, uint8_t pinEcho, Unit unit)
{
	this->pinTrigger = pinTrigger;
	this->pinEcho = pinEcho;
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
