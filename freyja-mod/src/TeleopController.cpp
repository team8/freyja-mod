#include <TeleopController.h>

TeleopController::TeleopController() :
	arm(),
	drivetrain(),
	lifter(),
	ramp(),
	driveJoystick((uint32_t) PORT_JOYSTICK_DRIVE),
	turnJoystick((uint32_t) PORT_JOYSTICK_TURN),
	operatorJoystick((uint32_t) PORT_JOYSTICK_OPERATOR)
{

}

void TeleopController::init() {
	arm->init();
	drivetrain->init();
	lifter->init();
	ramp->init();
}

void TeleopController::update(Robot *const robot) {
	// Drivetrain controls, will drive only if brake not being called
	// Brake will only be called if it is the first call
	bool previouslyBraking = false;
	if(driveJoystick.GetRawButton(1)) {
		if(!previouslyBraking) {
			drivetrain->brake();
		}
		previouslyBraking = true;
	}
	else {
		previouslyBraking = false;
		drivetrain->drive(turnJoystick.GetX(), driveJoystick.GetY());
	}

	arm->update();
	drivetrain->update();
	lifter->update();
	ramp->update();
}

void TeleopController::disable() {
	arm->disable();
	drivetrain->disable();
	lifter->disable();
	ramp->disable();
}

TeleopController::~TeleopController() {

}
