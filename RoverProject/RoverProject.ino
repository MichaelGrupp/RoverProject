/*
 Name:		RoverProject.ino
 Created:	29.07.2015 11:20:01
 Author:	Michael
*/
#include "drive.hpp"
#include "ultrasonicRanging.hpp"

Drive Chassis;
Ultrasonic USSensor(2, 3);
int IR_dist1 = 0;
int IR_dist2 = 0;
bool bumped = false;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
	USSensor.setUnit(Unit::CENTIMETERS);
}

// the loop function runs over and over again until power down or reset
void loop() {

  IR_dist1 = analogRead(3);
  IR_dist2 = analogRead(4);
  USSensor.measure();
  bumped = (USSensor.getDistance() > 5) && (USSensor.getDistance() < 25);
  Serial.println(USSensor.getDistance());

  if(bumped) {
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
  else if(IR_dist1 > 220) {
    Serial.println("bump 1");
    Chassis.driveCurved(220, Side::LEFT, Direction::FORWARD);
    delay(200);
  }
  else if(IR_dist2 > 220) {
    Serial.println("bump 2");
    Chassis.driveCurved(220, Side::RIGHT, Direction::FORWARD);
    delay(200);
  }
  else {
    Chassis.driveStraight(100, Direction::FORWARD);
    delay(150);
  }
    
}
