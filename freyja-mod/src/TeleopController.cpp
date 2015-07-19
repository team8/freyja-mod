#include <TeleopController.h>

TeleopController::TeleopController(Robot *robot) :
	robot(robot),
	driveJoystick((uint32_t) PORT_JOYSTICK_DRIVE),
	turnJoystick((uint32_t) PORT_JOYSTICK_TURN),
	operatorJoystick((uint32_t) PORT_JOYSTICK_OPERATOR),
	arm(&robot->arm),
	drivetrain(&robot->drivetrain),
	lifter(&robot->lifter),
	ramp(&robot->ramp),
	lifterLocked(false),
	wasOperatorTriggerPressed(false)
{

}

void TeleopController::init() {
	std::cout << "Lifter Init" << std::endl;
	lifter->init();
	std::cout << "Arm init" << std::endl;
	arm->init();
//	ramp->init();
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
	else if(operatorJoystick.GetRawButton(5)) {
//		lifter->levelUp();
	}
	//Moves down to nearest level 4
	else if(operatorJoystick.GetRawButton(4)) {
//		lifter->levelDown();
	}
	if(lifterLocked) {
		break;
	}
	else {
		lifter->setVelocity(operatorJoystick.GetY());
	}
}

void TeleopController::operateDrivetrain() {
	if(driveJoystick.GetTrigger()) {
		drivetrain->brake();
		return;
	}
	drivetrain->drive(turnJoystick.GetX(), driveJoystick.GetY());
}

void TeleopController::operateArm() {
	//Prevents pressing and holding repeatedly calling
	if(operatorJoystick.GetTrigger() && !wasOperatorTriggerPressed) {
		wasOperatorTriggerPressed = true;
		//arm->toggle();
	}
	else if(!operatorJoystick.GetTrigger()) {
		wasOperatorTriggerPressed = false;
	}
}

TeleopController::~TeleopController() {

}
