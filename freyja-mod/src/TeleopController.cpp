#include <TeleopController.h>

TeleopController::TeleopController() :
	arm(),
	drivetrain(),
	lifter(),
	ramp()
{

}

void TeleopController::init() {
	arm->init();
	drivetrain->init();
	lifter->init();
	ramp->init();
}

void TeleopController::update(Robot *const robot) {

}

void TeleopController::disable() {
	arm->disable();
	drivetrain->disable();
	lifter->disable();
	ramp->disable();
}

TeleopController::~TeleopController() {

}
