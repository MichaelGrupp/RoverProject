/*
 Name:		RoverProject.ino
 Created:	29.07.2015 11:20:01
 Author:	Michael
*/
#include "drive.hpp"

Drive Chassis;

// the setup function runs once when you press reset or power the board
void setup() {
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	Chassis.driveStraight(75, Direction::FORWARD);
	delay(3000);
	Chassis.stop();
	delay(1000);
	Chassis.driveStraight(75, Direction::BACKWARD);
	delay(3000);
	Chassis.stop();
	delay(1000);
	Chassis.turnLeft(100);
	delay(2000);
	delay(1000);
	Chassis.turnRight(100);
	delay(2000);
}
