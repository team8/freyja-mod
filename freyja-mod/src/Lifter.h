// Author: Nihar
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include <WPILib.h>
#include "Subsys.h"
#include "Ports.h"

/**
 * Represents Freyja's chain lifter with one Talon, one Encoder and two
 * Hall Effect sensors. The lifter can be controlled by the joystick or move
 * to specific levels.
 */

class Lifter: public Subsys {
public:
	/**
	 * The constructor
	 */
	Lifter();

	/**
	 * Initializes the lifter by resetting the encoders and controllers
	 */
	void init();

	/**
	 * Runs a passive state machine that will automatically switch
	 * away from an automated state. Also will check HFX sensors and bounce
	 * the lifter back if one is being hit
	 */
	void update();

	/**
	 * Disables the lifter stopping all controllers and robot code
	 */
	void disable();

	/**
	 * Sets the lifter state to IDLE and holds the Lifter in place with PID
	 */
	void idle();

	/**
	 * Returns true if the lifter state is idle
	 * @return true is the lifter state is idle
	 */
	bool isIdle();

	/**
	 * Used in teleop sets the velocity of the lifter taking into account
	 * a scalling factor and a minimum and maximum velocity. Sets the state to TELEOP
	 * @param velocity the input velocity
	 */
	void setVelocity(double velocity);

	/**
	 * Uses PID to set the lifter to a specific level. Sets the state to AUTOMATED
	 * @param level the desired level
	 */
	void setLevel(double level);

	/**
	 * Uses PID to lift the lifter an ammount of levels. Calls setLevel()
	 * @param liftAmount levels to be lifted
	 */
	void liftLevel(double liftAmount);

	/**
	 * Calls setLevel to position zero, returning the lifter to its original position
	 */
	void zero();

	/**
	 * Resets the zero of the lifter to the current position
	 */
	void resetZero();

private:
	//PID Constants
	const double PROPORTIONAL_CONSTANT = 0.0;
	const double INTEGRAL_CONSTANT = 0.0;
	const double DERIVATIVE_CONSTANT = 0.0;
	const double ENCODER_DPP = 0.0;

	//Height of a level in the unit used by encoders (in.)
	const double LEVEL_HEIGHT = 0.0;

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
	const double ENCODER_MAX_PERIOD = 0;


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

	/**
	 * Set the state to the input state as long as the current state is not DISABLED
	 * @param state new state to be set to
	 */
	void setState(State state);

	/**
	 * Returns true if the bottom limit switch is being triggered
	 * @return true if the bottom limit switch is being triggered
	 */
	bool isBottomHit();

	/**
	 * Returns true if the top limit switch is being triggered
	 * @return true if the top limit switch is being triggered
	 */
	bool isTopHit();
};
#endif /* SRC_LIFTER_H_ */
