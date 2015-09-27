#include <TeleopController.h>

TeleopController::TeleopController(Robot *robot) :
	robot(robot),
	driveJoystick((uint32_t) PORT_JOYSTICK_DRIVE),
	turnJoystick((uint32_t) PORT_JOYSTICK_TURN),
	operatorJoystick((uint32_t) PORT_JOYSTICK_OPERATOR),
	drivetrain(&robot->drivetrain),
	lifter(&robot->lifter),
	lifterLocked(false),
	wasOperatorTriggerPressed(false),
	wasDrivetrainTriggerPressed(false)
{

}

void TeleopController::init() {
	std::cout << "Lifter Init" << std::endl;
	lifter->init();
	std::cout << "DT Init" << std::endl;
	drivetrain->init(); // problem
	std::cout << "Init finished" << std::endl;
}

void TeleopController::update() {
	// DRIVETRAIN CONTROLS, will drive only if brake not being called
	// Brake will only be called if it is the first call
	operateDrivetrain();

	// LIFTER CONTROLS
	operateLifter();

	drivetrain->update();
	lifter->update();
}

void TeleopController::disable() {
	drivetrain->disable();
	lifter->disable();
}

void TeleopController::operateLifter() {
	//Unlocks the lifter
	if(operatorJoystick.GetRawButton(9)) {
		lifterLocked = false;
	}
	//Locks the lifter
	if(operatorJoystick.GetRawButton(8)) {
		lifterLocked = true;
	}
	//Zeroes the lifter
	if(operatorJoystick.GetRawButton(7)) {
		lifter->zero();
	}
	//Resets the zero
	else if(operatorJoystick.GetRawButton(6)) {
		lifter->resetZero();
	}
	//Moves up 1 level
	else if(operatorJoystick.GetRawButton(3)) {
		lifter->liftLevel(1);
	}
	//Moves down 1 level
	else if(operatorJoystick.GetRawButton(2)) {
		lifter->liftLevel(-1);
	}
	//Moves up to nearest level
	else if(operatorJoystick.GetRawButton(11)) {
		lifter->levelUp();
	}
	//Moves down to nearest level
	else if(operatorJoystick.GetRawButton(10)) {
		lifter->levelDown();
	}
	if(lifterLocked) {
		return;
	}
	else {
		lifter->setVelocity(operatorJoystick.GetY());
	}
}

void TeleopController::operateDrivetrain() {
	if(driveJoystick.GetRawButton(10)) {
		drivetrain->driveDist(100);
	}
	if(driveJoystick.GetRawButton(11)) {
		drivetrain->drive(0, 0);
	}

	drivetrain -> drive(turnJoystick.GetX(), driveJoystick.GetY());
	/*if(driveJoystick.GetTrigger() && !wasDrivetrainTriggerPressed) {
		drivetrain->brake();
		wasDrivetrainTriggerPressed = true;
		return;
	}
	if(!driveJoystick.GetTrigger()) {
		wasDrivetrainTriggerPressed = false;
		drivetrain->drive(turnJoystick.GetX(), driveJoystick.GetY());
	}*/
}

TeleopController::~TeleopController() {

}
