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
	drivetrain->drive(turnJoystick.GetX(), driveJoystick.GetY());
	lifter->setVelocity(operatorJoystick.GetY());
}

void TeleopController::disable() {
	arm->disable();
	drivetrain->disable();
	lifter->disable();
	ramp->disable();
}

TeleopController::~TeleopController() {

}
