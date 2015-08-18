/*
Odometry interface for the DFROBOT wheel encoders SEN0038

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

uint8_t Odometry::pulsesPerRotation = 10;
int Odometry::IDcount = 0;
unsigned long Odometry::avgSpeeds[4];	//meh
unsigned long Odometry::speeds[4];
int Odometry::pulses[4];
int Odometry::period = 100000; //period in microseconds


Odometry::Odometry(uint8_t interrupt)
{
	odometryParameters.ID = Odometry::IDcount;
	speeds[odometryParameters.ID] = 0;
	avgSpeeds[odometryParameters.ID] = 0;
	odometryParameters.pulseCount = 0;
	odometryParameters.currentSpeed = 0;
	odometryParameters.avgSpeed = 0;
	odometryParameters.state = State::RUNNING;
	attachInterrupt(interrupt, ISR_pulse, CHANGE);
	//Timer1 initialization
	if (Odometry::IDcount == 0) {
		Timer1.initialize(period); //default is 1 sec period
		Timer1.attachInterrupt(Odometry::ISR_timer);
	}
	Odometry::IDcount++;
}

void Odometry::setPulsesPerRotation(uint8_t pulsesPerRotation)
{
	Odometry::pulsesPerRotation = pulsesPerRotation;
}

OdometryParameters Odometry::getOdometryParameters()
{
	odometryParameters.pulseCount = pulses[odometryParameters.ID];
	odometryParameters.avgSpeed = avgSpeeds[odometryParameters.ID];
	odometryParameters.currentSpeed = speeds[odometryParameters.ID];
	return odometryParameters;
}

unsigned long Odometry::getCurrentSpeed()
{
	odometryParameters.currentSpeed = speeds[odometryParameters.ID];
	return odometryParameters.currentSpeed;
}

unsigned long Odometry::getAvgSpeed()
{
	odometryParameters.avgSpeed = avgSpeeds[odometryParameters.ID];
	return odometryParameters.avgSpeed;
}

void Odometry::start()
{
	odometryParameters.state = State::RUNNING;
}

void Odometry::pause()
{
	odometryParameters.state = State::PAUSED;
}

//void Odometry::ISR_pulse()
//{
//	if (odometryParameters.state == State::RUNNING) {
//		pulses[odometryParameters.ID]++; //object (local)
//		odometryParameters.pulseCount = pulses[odometryParameters.ID]; //static (global)
//	}
//}

void Odometry::ISR_pulse()
{
	for (int id = startID; id <= IDcount; id++) {
		pulses[id]++;
	}
}

void Odometry::ISR_timer()
{
	for (int id = startID; id <= IDcount; id++) {
		speeds[id] = (pulses[id] / pulsesPerRotation);
		avgSpeeds[id] = (avgSpeeds[id] + speeds[id]) / 2;
		pulses[id] = 0;
	}
	// Toggle LED
	digitalWrite(13, digitalRead(13) ^ 1);
}
