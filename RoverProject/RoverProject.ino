/*
 Name:		RoverProject.ino
 Created:	29.07.2015 11:20:01
 Author:	Michael
*/
#include "drive.hpp"
#include "ultrasonicRanging.hpp"
#include "TimerOne.h"
#include "odometry.hpp"

Odometry myOdometry(2, 3);   //interrupt 2 = digital pin 0
Drive Chassis;
Ultrasonic USSensor(2, 3);
uint8_t LeftMotorPower = 100;
uint8_t RightMotorPower = 100;
int IR_dist1 = 0;
int IR_dist2 = 0;
bool bumped = false;
int ledPin = 13;



// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	Timer1.initialize(myOdometry.getPeriod()); // set a timer of length x microseconds 
	Timer1.attachInterrupt(myOdometry.ISR_timer); // attach the service routine here
	pinMode(ledPin, OUTPUT);
	USSensor.setUnit(Unit::CENTIMETERS);
  Chassis.driveStraight(75, Direction::FORWARD);
  delay(500);
}

// the loop function runs over and over again until power down or reset
void loop() {

  Serial.println(myOdometry.getCurrentSpeed(Side::LEFT));
  Serial.println(myOdometry.getOdometryParameters(Side::LEFT).pulseCount);
  
	IR_dist1 = analogRead(3);
	IR_dist2 = analogRead(4);
	USSensor.measure();
	bumped = (USSensor.getDistance() > 5) && (USSensor.getDistance() < 15);
  digitalWrite(ledPin, bumped);
  
  //if ultrasound bumper or if stuck
	if (bumped || (myOdometry.getCurrentSpeed(Side::LEFT) == 0.0 && myOdometry.getCurrentSpeed(Side::RIGHT) == 0.0)) {
		Chassis.stop();
		delay(20);
		Chassis.driveStraight(75, Direction::BACKWARD);
		delay(1500);
		if (IR_dist1 > IR_dist2)
			Chassis.turnLeft(150);
		else
			Chassis.turnRight(150);
		delay(750);
    Chassis.driveStraight(100, Direction::FORWARD);
    delay(500);
	}
	else if (IR_dist1 > 170) {
		Chassis.driveCurved(140, Side::LEFT, Direction::FORWARD);
		delay(25);
	}
	else if (IR_dist2 > 170) {
		Chassis.driveCurved(150, Side::RIGHT, Direction::FORWARD);
		delay(25);
	}
	else {
    Chassis.driveStraight(85, Direction::FORWARD);
    delay(50);
	}

}
