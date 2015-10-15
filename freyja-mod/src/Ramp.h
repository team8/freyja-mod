/*
 * Ramp.h
 *
 *  Created on: Oct 1, 2015
 *      Author: Freddy
 */

#ifndef SRC_RAMP_H_
#define SRC_RAMP_H_

#define LEFT_SPEED 1.0
#define RIGHT_SPEED 1.0

#define DEPLOY_TIME 1.0

#include <WPILib.h>
#include "Subsys.h"

/**
 * The ramp is a subsystem of freyja-mod intended to allow the use of the chutedoor
 * It has flywheels whose purpose is to control the inflow of totes into the arm
 *
 */
class Ramp: public Subsys {
private:
	/** The right victor of the ramp **/
	Victor rightVic;

	/** The left victor of the ramp **/
	Victor leftVic;

	/** Boolean determining the ramp state for toggling **/
	bool rampToggled;

	/** The timer used to measure deploy time **/
	Timer runTime;

	/** The boolean determing whether or not the wheels are running **/
	enum State{
		IDLE,
		RUNNING,
		SLOWING,
//		DEPLOYING
	};
	State state;
public:
	/**
	 * Default constructor
	 */
	Ramp();

	/**
	 * Initializes the ramp and its components
	 */
	void init();

	/**
	 * This method is called continuously to keep the ramp updated and used to passively
	 */
	void update();

	/**
	 * Disables the ramp and its components
	 */
	void disable();

	/**
	 * Toggles the state of the ramp
	 */
//	void toggleRampDeploy();

	/**
	 * Toggles the state of the ramp
	 */
	void toggleRampSlow();

	/**
	 * Resets the toggle state of the ramp
	 */
	void toggleRampReset();

	/**
	 * Starts turning the wheels and manages their spin
	 */
	void start();

	/**
	 * Reverses the wheel speed to stop an incoming tote
	 */
	void slow();

	/**
	 * Stops the turning of the wheels and
	 */
	void stop();

	/**
	 * Stops the turning of the wheels and
	 */
	void deploy();

	/**
	 * Sets the state of the ramp
	 */
	void setState(State state);

	/**
	 * Idles the ramp
	 */
	void idle();

	/**
	 * Determines whether or not the ramp is idle
	 */
	bool isIdle();

	/**
	 * Returns whether or not the ramp has been running long enough to deploy
	 */
	bool hasDeployed();
};

#endif /* SRC_RAMP_H_ */
