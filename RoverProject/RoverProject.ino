/*
 Name:		RoverProject.ino
 Created:	29.07.2015 11:20:01
 Author:	Michael
*/
#include "drive.hpp"
#include "ultrasonicRanging.hpp"
#include "TimerOne.h"
#include "odometry.hpp"

Odometry LeftOdometry(2);   //interrupt 2 = digital pin 0
Odometry RightOdometry(3);  //interrupt 2 = digital pin 0
Drive Chassis;
Ultrasonic USSensor(2, 3);
uint8_t LeftMotorPower = 0;
uint8_t RightMotorPower = 0;
int IR_dist1 = 0;
int IR_dist2 = 0;
bool bumped = false;
int ledPin = 13;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	USSensor.setUnit(Unit::CENTIMETERS);
}

// the loop function runs over and over again until power down or reset
void loop() {

	IR_dist1 = analogRead(3);
	IR_dist2 = analogRead(4);
	USSensor.measure();
	bumped = (USSensor.getDistance() > 5) && (USSensor.getDistance() < 15);
	Serial.println(USSensor.getDistance());

	if (bumped) {
		Serial.println("bump 3");
		Chassis.stop();
		delay(50);
		Chassis.driveStraight(75, Direction::BACKWARD);
		delay(2000);
		if (IR_dist1 > IR_dist2)
			Chassis.turnLeft(150);
		else
			Chassis.turnRight(150);
		delay(2000);
	}
	else if (IR_dist1 > 180) {
		Serial.println("bump 1");
		Chassis.driveCurved(220, Side::LEFT, Direction::FORWARD);
		delay(200);
	}
	else if (IR_dist2 > 180) {
		Serial.println("bump 2");
		Chassis.driveCurved(220, Side::RIGHT, Direction::FORWARD);
		delay(200);
	}
	else {
		//test odometry: set speed to roundabout 1 rotations per second
		for (;;) {
			if (LeftOdometry.getCurrentSpeed() < 0.5) {
				LeftMotorPower = Chassis.getDriveParameters(Side::LEFT).power + 10;
				Chassis.setDriveParameters_L(LeftMotorPower, Direction::FORWARD);
			}
			if (RightOdometry.getCurrentSpeed() < 0.5) {
				RightMotorPower = Chassis.getDriveParameters(Side::RIGHT).power + 10;
				Chassis.setDriveParameters_R(RightMotorPower, Direction::FORWARD);
			}
			else {
				break;
			}
			Chassis.drive();
      Serial.println(LeftOdometry.getCurrentSpeed());
			delay(500);
		}
		//status LED for debugging
		//digitalWrite(ledPin, HIGH);
		//delay(500);
		//digitalWrite(ledPin, LOW);
	}

}
