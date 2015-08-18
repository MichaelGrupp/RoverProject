/*
Drive interface for DFROBOT Pirate 4WD mobile robot platform
(PWM control mode)

Controller board: DFROBOT Romeo v2

16.08.2015 Michael Grupp
*/


#include "drive.hpp"


void Drive::setOutput(DriveParameters driveParameters)
{
	//Define signal for direction
	auto dir = HIGH;
	if (driveParameters.dir == Direction::BACKWARD)
		dir = LOW;
	//Output to motors
	if (driveParameters.side == Side::LEFT) {
		digitalWrite(pinM2, dir);
		analogWrite(pinE2, driveParameters.speed);
	}
	else if (driveParameters.side == Side::RIGHT) {
		digitalWrite(pinM1, dir);
		analogWrite(pinE1, driveParameters.speed);
	}
}

Drive::Drive()
{
	//Pin setup
	for (int i = 4; i <= 7; i++)
		pinMode(i, OUTPUT);
	//Initialization of drive parameters
	motors_L.side = Side::LEFT;
	motors_L.dir = Direction::FORWARD;
	motors_L.speed = 0;
	motors_R.side = Side::RIGHT;
	motors_R.dir = Direction::FORWARD;
	motors_R.speed = 0;
}

void Drive::setDriveParameters_L(uint8_t speed, Direction dir)
{
	motors_L.speed = speed;
	motors_L.dir = dir;
}

void Drive::setDriveParameters_R(uint8_t speed, Direction dir)
{
	motors_R.speed = speed;
	motors_R.dir = dir;
}

void Drive::drive()
{
	setOutput(motors_L);
	setOutput(motors_R);
}

void Drive::stop()
{
	motors_L.speed = 0;
	motors_R.speed = 0;
	drive();
}

void Drive::driveStraight(uint8_t speed, Direction dir)
{
	motors_L.speed = speed;
	motors_R.speed = speed;
	motors_L.dir = dir;
	motors_R.dir = dir;
	drive();
}

void Drive::driveCurved(uint8_t speed, Side side, Direction dir)
{
	if (side == Side::LEFT) {
		motors_L.speed = 0;
		motors_R.speed = speed;
	}
	else {
		motors_L.speed = speed;
		motors_R.speed = 0;
	}
	motors_L.dir = dir;
	motors_R.dir = dir;
	drive();
}

void Drive::turnLeft(uint8_t speed)
{
	motors_L.speed = speed;
	motors_R.speed = speed;
	motors_L.dir = Direction::BACKWARD;
	motors_R.dir = Direction::FORWARD;
	drive();
}

void Drive::turnRight(uint8_t speed)
{
	motors_L.speed = speed;
	motors_R.speed = speed;
	motors_L.dir = Direction::FORWARD;
	motors_R.dir = Direction::BACKWARD;
	drive();
}
