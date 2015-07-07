/*
 * Lifter.h
 *
 *  Created on: Jun 23, 2015
 *      Author: Nihar
 */

#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include <WPILib.h>
#include "Subsys.h"
#include "Ports.h"

/**
 * Represents Freyja's chain lifter
 */

class Lifter: public Subsys {
	public:
		Lifter();
		void init();
		void update();
		void disable();
		void idle();
		bool isIdle();
		void setVelocity(double velocity);
		void setLevel(double level);
		void zero();
	private:
		/*
		 * States that lifter can be in
		 */
		enum State {
			IDLE,
			TELEOP,
			AUTOMATED,
			DISABLED
		};
		//Height of a level in the unit used by encoders (in.)
		const int LEVEL_HEIGHT = 1;
		//PID Constants
		const double PROPORTIONAL = 1;
		const double INTEGRAL = 1;
		const double DERIVATIVE = 1;
		const double DPP = 1;
		const double ACCEPTABLE_PID_ERROR = 1;
		//Speed when bouncing due to Hall effect trigger
		const double BOUNCE_SPEED = 1;
		//Joystick control related variables/constants
		const double MAX_SPEED = 1;
		const double SPEED_SCALING = 1;

		//Encoder and PID Controller for position/velocity
		Encoder encoder;
		PIDController controller;

		//Talon that drives the lifter
		TalonSRX talon;

		//Hall effect sensors
		DigitalInput topSensor;
		DigitalInput bottomSensor;

		//Current level
		double currentLevel;
		State state;
		void setState(State state);
		bool isBottomHit();
		bool isTopHit();
		//somehow do limit switch hall effect
};
#endif /* SRC_LIFTER_H_ */
