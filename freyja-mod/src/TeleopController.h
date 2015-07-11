#ifndef TELEOP_CONTROLLER_H
#define TELEOP_CONTROLLER_H

#include "WPILib.h"
#include "Robot.h"
#include "paths.h"
#include "Arm.h"
#include "Drivetrain.h"
#include "Lifter.h"
#include "Ramp.h"

class TeleopController {
public:

	TeleopController();
	~TeleopController();

	/**
	 * Currently doesn't actually do anything
	 */
	void init();

	/**
	 * Commands the robot's actions based on human input
	 * @param robot The pointer to the robot which will be controlled by the TeleController
	 */
	void update(Robot *const robot);

	void disable();

private:
	// The four subsystems
	Arm *arm;
	Drivetrain *drivetrain;
	Lifter *lifter;
	Ramp *ramp;


	Joystick driveJoystick;
	Joystick turnJoystick;
	Joystick operatorJoystick;
};

#endif
