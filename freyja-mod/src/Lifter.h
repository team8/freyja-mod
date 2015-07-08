/*
 * Author: Nihar
 */
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include <WPILib.h>
#include "Subsys.h"
#include "Ports.h"

//Height of a level in the unit used by encoders (in.)
#define LEVEL_HEIGHT 0.0

//PID Constants
#define PROPORTIONAL 0.0
#define INTEGRAL 0.0
#define DERIVATIVE 0.0
#define DPP 0.0

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
		void liftLevel(double liftAmount);
		void zero();
		void resetZero();

	private:
		// Error to define when PID is complete
		const double ACCEPTABLE_PID_ERROR = 0;

		// Speed when bouncing due to Hall effect trigger
		const double BOUNCE_SPEED = 0;

		// Maximum Lifter Speed
		const double MAX_SPEED = 0;

		// Scaling value for teleop control
		const double SPEED_SCALING = 0;

		// Bounds on PID input values
		const double INPUT_RANGE = 0;

		//Talon that drives the lifter
		TalonSRX talon;

		//Encoder on talon
		Encoder encoder;

		// Controller that acts on talon and encoder
		PIDController controller;

		// Hall effect sensors for top of the elevator
		DigitalInput topSensor;

		// Hall effect sensors for bottom of the elevator
		DigitalInput bottomSensor;

		// Current level
		double currentLevel;

		/*
		 * States that lifter can be in
		 *
		 * IDLE - Runs velocity PID to maintain position
		 * TELEOP - Scales joystick input to run talon
		 * AUTOMATED - Runs positon PID to change level
		 * DISABLED - Unoperational for testing other things
		 */
		enum State {
			IDLE,
			TELEOP,
			AUTOMATED,
			DISABLED
		} state;

		void setState(State state);
		bool isBottomHit();
		bool isTopHit();
};
#endif /* SRC_LIFTER_H_ */
