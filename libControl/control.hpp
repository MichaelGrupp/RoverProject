/*
Control library (PID)

x --> (calculateError) --e--> [calculateControlCommand] --u--> [*Actor*] --> y
              |                                                           |
			  |                                                           |
              ---------------------------*Sensor*--------------------------

19.08.2015 Michael Grupp
*/

#include "Arduino.h"


#ifndef _LIBCONTROL_
#define _LIBCONTROL_


template<typename K_type, typename X_type, typename U_type>
class Control
{
private:
	//Control gains
	K_type Kp;
	K_type Kd;
	K_type Ki;

	X_type x;			//desired value
	X_type lastInput;	//for D
	X_type sumInput;	//for I
public:
	//Constructors
	Control() {
		Kp = 1;
		Ki = 1;
		Kd = 1;
		lastInput = 0;
	}
	Control(K_type Kp, K_type Kd, K_type Ki) {
		this->Kp = Kp;
		this->Kd = Kd;
		this->Ki = Ki;
		lastInput = 0;
	}
	Control(K_type Kp, K_type Kd, K_type Ki, X_type x) {
		this->Kp = Kp;
		this->Kd = Kd;
		this->Ki = Ki;
		this->x = x;
	}

	//setters
	void setControlGains(K_type Kp, K_type Kd, K_type Ki) {
		this->Kp = Kp;
		this->Kd = Kd;
		this->Ki = Ki;
	}
	void setDesiredValue(X_type x) {
		this->x = x;
	}

	//Control functions
	X_type calculateError(X_type input) {
		return (x - input);
	}
	U_type calculateControlCommand(X_type input, int dt) {
		sumInput += input;
		X_type e = calculateError(input);
		U_type u = (Kp*e) + (Kd*(input - lastInput) / dt) + Ki(sumInput*dt);
		lastInput = input;
		return u;
	}

};

/*
	template definitions must be in .hpp file
*/

#endif;
