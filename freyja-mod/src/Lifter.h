// Author: Nihar
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "WPILib.h"
#include "Subsys.h"
#include "Ports.h"

/**
 * Represents Freyja's chain lifter with one Victor, one Encoder and two
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
	 * Uses PID to lift the lifter an amount of levels. Calls setLevel()
	 * @param liftAmount levels to be lifted
	 */
	void liftLevel(double liftAmount);

	/**
	 * Uses PID to move the lifter to the closest upper level
	 */
	void levelUp();

	/**
	 * Uses PID to move the lifter to the closest lower level
	 */
	void levelDown();

	/**
	 * Calls setLevel to position zero, returning the lifter to its original position
	 */
	void zero();

	/**
	 * Resets the zero of the lifter to the current position
	 */
	void resetZero();

	/**
	 * Disables all PID controllers
	 */
	void disableControllers();


private:
	//PID Constants
	// The ultimate gain is 2.07
	// Oscillation period is 0.03
	// Do not change the PID values

	// Minimal Overshoot Values:
	// P: 0.414
	// I: 0.015
	// D: 0.010

	// Quarter Wave Decay Values
	// P: 1.242
	// I: 0.015
	// D: 0.00375

	const double PROPORTIONAL_CONSTANT = 1.242;
	const double INTEGRAL_CONSTANT = 0.015;
	const double DERIVATIVE_CONSTANT = 0.00375;

	//PID Constants - Velocity
	const double VELOCITY_PROPORTIONAL_CONSTANT = 0.00012;
	const double VELOCITY_INTEGRAL_CONSTANT = 0.0000;
	const double VELOCITY_DERIVATIVE_CONSTANT = 0.0001;

	const double ENCODER_DPP = 0.022441;

	//Height of a level in the unit used by encoders (in.)
	const double LEVEL_HEIGHT = 12.0;

	// Error to define when PID is complete
	const double ACCEPTABLE_PID_ERROR = 0.5;

	// Speed when bouncing due to Hall effect trigger
	const double BOUNCE_SPEED = 0.4;

	// Maximum Lifter Speed
	const double MAX_SPEED = 0.8; //0.3;

	const double PID_MAX_SPEED = 0.3;

	// Scaling value for teleop control
	const double SPEED_SCALING = 1;

	// Bounds on PID input values
	const double INPUT_RANGE = 9999;
	const double ENCODER_MAX_PERIOD = 100;

	//
	double predictedSpeed;

	// Victor that drives the lifter
	Victor victor1;

	// Other victor that drives the lifter
	Victor victor2;

	//Encoder on victor
	Encoder encoder;

	// Distance PID Controller that acts on victor and encoder
	PIDController distanceController1;
	// Acts on other victor and encoder
	PIDController distanceController2;

	// Hall effect sensors for top of the elevator
	DigitalInput topSensor;

	// Hall effect sensors for bottom of the elevator
	DigitalInput bottomSensor;

	// Velocity PID Controller
	PIDController velocityController1;
	PIDController velocityController2;


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

	/**
	 * Prints out debug
	 */
	void debug();
};
#endif /* SRC_LIFTER_H_ */
