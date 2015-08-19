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

#include "Arduino.h"


#ifndef _ODOMETRY_
#define _ODOMETRY_

#ifndef _SIDE_
#define _SIDE_
enum class Side {
	LEFT, RIGHT
};
#endif

enum class State {
	RUNNING, PAUSED
};

typedef struct OdometryParameters {
	Side side;
	uint8_t ID;
	State state;
	int pulseCount;	//(current)
	float currentSpeed;//rotations per second
	float avgSpeed;
};

class Odometry
{
private:
	//Pins
	uint8_t interrupt;	//Interrupt number of board (see top of Odometry.hpp file)

	//Odometry Parameters
	OdometryParameters odometryParameters_L;
	OdometryParameters odometryParameters_R;

	//Encoder characteristics
	static uint8_t pulsesPerRotation;

	// ISR stuff must be handled static
	static long period;
	static float pulsesRight, pulsesLeft;
	static float speeds[];
	static float avgSpeeds[];

public:
	//Constructor
	//interrupt: the interrupt number you want to use (depends on board, see top of Odometry.hpp file)
	Odometry(uint8_t interruptLeft, uint8_t interruptRight);

	//setters
	static void setPulsesPerRotation(uint8_t pulsesPerRotation);

	//getters
	unsigned long getPeriod();
	OdometryParameters getOdometryParameters(Side side);
	unsigned long getCurrentSpeed(Side side);
	unsigned long getAvgSpeed(Side side);

	//Control functions
	void start();
	void pause();

	//Interrupt service routines (ISR) for measurement
	static void ISR_pulseLeft();//pulse counting
	static void ISR_pulseRight();
	static void ISR_timer();	//speed measurement
};

#endif;