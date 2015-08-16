/*
Drive interface for DFROBOT Pirate 4WD mobile robot platform
(PWM control mode)

TODO: evaluate PLL mode

Controller board: DFROBOT Romeo v2

16.08.2015 Michael Grupp
*/

#include "Arduino.h"


#ifndef _DRIVE_
#define _DRIVE_

enum class Direction {
	FORWARD, BACKWARD
};

enum class Side {
	LEFT, RIGHT
};

typedef struct DriveParameters {
	Side side;
	uint8_t speed;
	Direction dir;
};

class Drive
{
private:
	//Pins
	uint8_t pinE1 = 5;	//Motor speed 1 (right side)
	uint8_t pinE2 = 6;	//Motor speed 2 (left side)
	uint8_t pinM1 = 4;	//Motor direction 1
	uint8_t pinM2 = 7;	//Motor direction 2

	//Drive parameters for left and right
	DriveParameters motors_L;
	DriveParameters motors_R;

	//Low level access to motors
	void setOutput(DriveParameters driveParameters);

public:
	//Constructors
	Drive();

	//setters
	void setDriveParameters_L(uint8_t speed, Direction dir);
	void setDriveParameters_R(uint8_t speed, Direction dir);

	//Drive with current settings
	void drive();

	//Predefined drive functions
	void stop();
	void driveStraight(uint8_t speed, Direction dir);
	void turnLeft(uint8_t speed);
	void turnRight(uint8_t speed);

};

#endif;