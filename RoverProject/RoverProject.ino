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
	Timer1.initialize(myOdometry.getPeriod()); // set a timer of length x microseconds 
	Timer1.attachInterrupt(myOdometry.ISR_timer); // attach the service routine here
	pinMode(ledPin, OUTPUT);
	USSensor.setUnit(Unit::CENTIMETERS);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	IR_dist1 = analogRead(3);
	IR_dist2 = analogRead(4);
	USSensor.measure();
	bumped = (USSensor.getDistance() > 5) && (USSensor.getDistance() < 25);

	if (bumped) {
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
		Chassis.driveCurved(250, Side::LEFT, Direction::FORWARD);
		delay(200);
	}
	else if (IR_dist2 > 180) {
		Chassis.driveCurved(250, Side::RIGHT, Direction::FORWARD);
		delay(200);
	}
	else {
    Chassis.setDriveParameters_L(LeftMotorPower, Direction::FORWARD);
    Chassis.setDriveParameters_R(RightMotorPower, Direction::FORWARD);
    Chassis.drive();
    delay(100);
		//test odometry: set speeds of each wheel to roundabout x rotations per second
	  if (myOdometry.getCurrentSpeed(Side::LEFT) < 0.2 && LeftMotorPower < 245) {
				LeftMotorPower = Chassis.getDriveParameters(Side::LEFT).power + 5;
				Chassis.setDriveParameters_L(LeftMotorPower, Direction::FORWARD);
			}
		if (myOdometry.getCurrentSpeed(Side::RIGHT) < 0.2 && RightMotorPower < 245) {
				RightMotorPower = Chassis.getDriveParameters(Side::RIGHT).power + 5;
				Chassis.setDriveParameters_R(RightMotorPower, Direction::FORWARD);
			}
		if (myOdometry.getCurrentSpeed(Side::LEFT) > 0.22) {
        LeftMotorPower = Chassis.getDriveParameters(Side::LEFT).power - 5;
        Chassis.setDriveParameters_L(LeftMotorPower, Direction::FORWARD);
		}
    if (myOdometry.getCurrentSpeed(Side::RIGHT) > 0.22) {
        RightMotorPower = Chassis.getDriveParameters(Side::RIGHT).power - 5;
        Chassis.setDriveParameters_R(RightMotorPower, Direction::FORWARD);
    }
    Chassis.drive();
	}

}
