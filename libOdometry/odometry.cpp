/*
Odometry interface for the DFROBOT wheel encoders SEN0038

!!Can not be used while using the Serial com interface !! (same interrupt)

Interrupts on DFROBOT Romeo v2 / Arduino Leonardo:
(https://www.arduino.cc/en/Reference/AttachInterrupt)
int 0	-	dig pin 3
int 1	-	dig pin 2
int 2	-	dig pin 0
int 3	-	dig pin	1
int 4	-	dig pin 7

18.08.2015 Michael Grupp
*/

#include "odometry.hpp"
#include "TimerOne.h"	//external library http://playground.arduino.cc/Code/Timer1

#define leftID 0
#define rightID 1

extern TimerOne Timer1;

//static instantiations
uint8_t Odometry::pulsesPerRotation = 10;
unsigned long Odometry::avgSpeeds[2] = { 0,0 };
unsigned long Odometry::speeds[2] = { 0,0 };
int Odometry::pulsesLeft = 0;
int Odometry::pulsesRight = 0;
long Odometry::period = 100000;

Odometry::Odometry(uint8_t interruptLeft, uint8_t interruptRight)
{
	odometryParameters_L.side = Side::LEFT;
	odometryParameters_L.ID = leftID;
	odometryParameters_L.pulseCount = 0;
	odometryParameters_L.currentSpeed = 0;
	odometryParameters_L.avgSpeed = 0;
	odometryParameters_L.state = State::RUNNING;
	attachInterrupt(interruptLeft, ISR_pulseLeft, CHANGE);
	odometryParameters_R.side = Side::RIGHT;
	odometryParameters_R.ID = rightID;
	odometryParameters_R.pulseCount = 0;
	odometryParameters_R.currentSpeed = 0;
	odometryParameters_R.avgSpeed = 0;
	odometryParameters_R.state = State::RUNNING;
	attachInterrupt(interruptRight, ISR_pulseRight, CHANGE);
	//Timer1 initialization
	//period = 1000000; //period in microseconds
	//Timer1.initialize(period); //default is 1 sec period
	//Timer1.attachInterrupt(ISR_timer, period);
}

void Odometry::setPulsesPerRotation(uint8_t pulsesPerRotation)
{
	Odometry::pulsesPerRotation = pulsesPerRotation;
}

unsigned long Odometry::getPeriod()
{
	return period;
}

OdometryParameters Odometry::getOdometryParameters(Side side)
{
	if (side == Side::LEFT) {
		odometryParameters_L.pulseCount = pulsesLeft;
		odometryParameters_L.avgSpeed = avgSpeeds[odometryParameters_L.ID];
		odometryParameters_L.currentSpeed = speeds[odometryParameters_L.ID];
		return odometryParameters_L;
	}
	else {
		odometryParameters_R.pulseCount = pulsesRight;
		odometryParameters_R.avgSpeed = avgSpeeds[odometryParameters_R.ID];
		odometryParameters_R.currentSpeed = speeds[odometryParameters_R.ID];
		return odometryParameters_R;
	}
}

unsigned long Odometry::getCurrentSpeed(Side side)
{
	if (side == Side::LEFT) {
		odometryParameters_L.currentSpeed = speeds[odometryParameters_L.ID];
		return odometryParameters_L.currentSpeed;
	}
	else {
		odometryParameters_R.currentSpeed = speeds[odometryParameters_R.ID];
		return odometryParameters_R.currentSpeed;
	}
}

unsigned long Odometry::getAvgSpeed(Side side)
{
	if (side == Side::LEFT) {
		odometryParameters_L.avgSpeed = avgSpeeds[odometryParameters_L.ID];
		return odometryParameters_L.avgSpeed;
	}
	else {
		odometryParameters_R.avgSpeed = avgSpeeds[odometryParameters_R.ID];
		return odometryParameters_R.avgSpeed;
	}
}

void Odometry::start()
{
	odometryParameters_L.state = State::RUNNING;
	odometryParameters_R.state = State::RUNNING;
}

void Odometry::pause()
{
	odometryParameters_L.state = State::PAUSED;
	odometryParameters_R.state = State::PAUSED;
}

void Odometry::ISR_pulseLeft()
{
	Odometry::pulsesLeft++;
	// Toggle LED (debug)
	digitalWrite(13, digitalRead(13) ^ 1);
}

void Odometry::ISR_pulseRight()
{
	Odometry::pulsesRight++;
	// Toggle LED (debug)
	digitalWrite(13, digitalRead(13) ^ 1);
}

void Odometry::ISR_timer()
{
	speeds[leftID] = ((pulsesLeft / pulsesPerRotation)*10) / 2;
	avgSpeeds[leftID] = (avgSpeeds[leftID] + speeds[leftID]) / 2;
	pulsesLeft = 0;
	speeds[rightID] = ((pulsesRight / pulsesPerRotation)*10) / 2;
	avgSpeeds[rightID] = (avgSpeeds[rightID] + speeds[rightID]) / 2;
	pulsesRight = 0;
	// Toggle LED (debug)
	digitalWrite(13, digitalRead(13) ^ 1);
}