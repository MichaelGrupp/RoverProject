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

#include "Arduino.h"


#ifndef _ODOMETRY_
#define _ODOMETRY_


enum class State {
	RUNNING, PAUSED
};

typedef struct OdometryParameters {
	uint8_t ID;
	State state;
	int pulseCount;	//(current)
	unsigned long currentSpeed;//rotations per second
	unsigned long avgSpeed;
};

class Odometry
{
private:
	//Pins
	uint8_t interrupt;	//Interrupt number of board (see top of Odometry.hpp file)

	//Odometry Parameters
	OdometryParameters odometryParameters;

	//Encoder characteristics
	static uint8_t pulsesPerRotation;

	// ISR stuff must be handled static
	const static uint8_t startID = 0;
	static int IDcount;
	static int period;
	static int pulses[];
	static unsigned long speeds[];
	static unsigned long avgSpeeds[];

public:
	//Constructor
	//interrupt: the interrupt number you want to use (depends on board, see top of Odometry.hpp file)
	Odometry(uint8_t interrupt);

	//setters
	static void setPulsesPerRotation(uint8_t pulsesPerRotation);

	//getters
	OdometryParameters getOdometryParameters();
	unsigned long getCurrentSpeed();
	unsigned long getAvgSpeed();

	//Control functions
	void start();
	void pause();

	//Interrupt service routines (ISR) for measurement
	static void ISR_pulse();	//pulse counting
	static void ISR_timer();	//speed measurement
};

#endif;