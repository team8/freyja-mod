#ifndef TELEOP_CONTROLLER_H
#define TELEOP_CONTROLLER_H

#include "WPILib.h"
#include "Robot.h"
#include "paths.h"
#include "Drivetrain.h"
#include "Lifter.h"

class TeleopController {
public:

	TeleopController(Robot *const robot);
	~TeleopController();

	/**
	 * Currently doesn't actually do anything
	 */
	void init();

	/**
	 * Commands the robot's actions based on human input
	 * @param robot The pointer to the robot which will be controlled by the TeleController
	 */
	void update();

	void disable();

private:
	Robot *robot;

	Joystick driveJoystick;
	Joystick turnJoystick;
	Joystick operatorJoystick;

	Drivetrain *drivetrain;
	Lifter *lifter;

	//Stores if lifter is locked (joystick controls disabled)
	bool lifterLocked;
	bool wasOperatorTriggerPressed;
	bool wasDrivetrainTriggerPressed;
	//Runs the lifter according to joystick input
	void operateLifter();
	//Runs the drivetrain using joystick input
	void operateDrivetrain();
};

#endif
