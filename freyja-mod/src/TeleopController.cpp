#include <TeleopController.h>

TeleopController::TeleopController(Robot *robot) :
	robot(robot),
	driveJoystick((uint32_t) PORT_JOYSTICK_DRIVE),
	turnJoystick((uint32_t) PORT_JOYSTICK_TURN),
	operatorJoystick((uint32_t) PORT_JOYSTICK_OPERATOR),
	drivetrain(&robot->drivetrain),
	lifter(&robot->lifter),
	ramp(&robot->ramp),
	lifterLocked(false),
	wasOperatorTriggerPressed(false),
	wasDrivetrainTriggerPressed(false),
	rampDelay()
{

}

void TeleopController::init() {
	std::cout << "Lifter Init" << std::endl;
	lifter->init();
	std::cout << "DT Init" << std::endl;
	drivetrain->init(); // problem
	std::cout << "Ramp Init" << std::endl;
	ramp->init();
	std::cout << "Init Over" << std::endl;
}

void TeleopController::update() {
	// DRIVETRAIN CONTROLS, will drive only if brake not being called
	// Brake will only be called if it is the first call
	operateDrivetrain();

	// LIFTER CONTROLS
	operateLifter();

	//RAMP CONTROLS
	operateRamp();

	drivetrain->update();
	lifter->update();
	ramp->update();
}

void TeleopController::disable() {
	drivetrain->disable();
	lifter->disable();
	ramp->disable();
}

void TeleopController::operateRamp() {
	//Toggles the ramp state
	if(operatorJoystick.GetRawButton(2)) {
		ramp->toggleRampSlow();
		rampDelay.Reset();
		rampDelay.Start();
		return;
	}
	//Deploys a prepared tote
	if(operatorJoystick.GetRawButton(1) && rampDelay.Get() > DELAY_TIME) {
		ramp->toggleRampDeploy();
		rampDelay.Stop();
		//Do not reset otherwise you can't go forward consecutively repeatedly
		return;
	}
	ramp->toggleRampReset();
}

void TeleopController::operateLifter() {
	//Unlocks the lifter
	if(operatorJoystick.GetRawButton(8)) {
		lifterLocked = false;
		lifter->disableControllers();
	}
	//Locks the lifter
	if(operatorJoystick.GetRawButton(9)) {
		lifterLocked = true;
		lifter->disableControllers();
	}

	//Resets the zero
//	else if(operatorJoystick.GetRawButton()) {
//		lifter->resetZero();
//	}
	//Moves up 1 level
	else if(operatorJoystick.GetRawButton(7)) {
		lifter->liftLevel(-1.0);
	}
	//Moves down 1 level
	else if(operatorJoystick.GetRawButton(6)) {
		lifter->liftLevel(1.0);
	}
	//Moves up to nearest level
//	else if(operatorJoystick.GetRawButton(6)) {
//		lifter->levelUp();
//	}
//	//Moves down to nearest level
//	else if(operatorJoystick.GetRawButton(5)) {
//		lifter->levelDown();
//	}
	if(operatorJoystick.GetRawButton(12)) {
		lifter->idle();
	}
	if(!lifterLocked){
		lifter->setVelocity(operatorJoystick.GetY());
	}
}

void TeleopController::operateDrivetrain() {
	if(driveJoystick.GetTrigger() && !wasDrivetrainTriggerPressed) {
		drivetrain->brake();
		wasDrivetrainTriggerPressed = true;
		return;
	}
	if(!driveJoystick.GetTrigger()) {
		wasDrivetrainTriggerPressed = false;
		drivetrain->drive(turnJoystick.GetX(), driveJoystick.GetY());
	}
}

TeleopController::~TeleopController() {

}
