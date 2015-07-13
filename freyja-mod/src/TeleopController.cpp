#include <TeleopController.h>

TeleopController::TeleopController(Robot *robot) :
	robot(robot),
	driveJoystick((uint32_t) PORT_JOYSTICK_DRIVE),
	turnJoystick((uint32_t) PORT_JOYSTICK_TURN),
	operatorJoystick((uint32_t) PORT_JOYSTICK_OPERATOR),
	arm(&robot->arm),
	drivetrain(&robot->drivetrain),
	lifter(&robot->lifter),
	ramp(&robot->ramp)
{

}

void TeleopController::init() {
/*	std::cout << "TeleCtrlr Init Part 1" << std::endl;
	lifter->init();
	std::cout << "TeleCtrlr Init Part 2" << std::endl;
	arm->init();
	std::cout << "TeleCtrlr Init Part 3" << std::endl;
	ramp->init();*/
	std::cout << "TeleCtrlr Init Part 1" << std::endl;
	drivetrain->init(); // problem
	std::cout << "TeleCtrlr Init Part 2" << std::endl;
}

void TeleopController::update() {
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
