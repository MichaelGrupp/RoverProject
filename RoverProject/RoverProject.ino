/*
 Name:		RoverProject.ino
 Created:	29.07.2015 11:20:01
 Author:	Michael
*/
#include "drive.hpp"
#include "ultrasonicRanging.hpp"

Drive Chassis;
Ultrasonic USSensor(2, 3);

// the setup function runs once when you press reset or power the board
void setup() {
	USSensor.setUnit(Unit::CENTIMETERS);
}

// the loop function runs over and over again until power down or reset
void loop() {
	USSensor.measure();
	while (USSensor.inRange() && USSensor.getDistance() > 20 || USSensor.getDistance() == 0) {
		Chassis.driveStraight(75, Direction::FORWARD);
		delay(25);
		USSensor.measure();
	}
	Chassis.stop();
	delay(200);
	Chassis.driveStraight(75, Direction::BACKWARD);
	delay(3000);
	Chassis.stop();
	delay(200);
	Chassis.turnLeft(100);
	delay(2000);
	Chassis.turnRight(100);
	delay(2000);
}
