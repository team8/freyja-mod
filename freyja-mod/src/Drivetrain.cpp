#include <Drivetrain.h>

Drivetrain::Drivetrain() :
leftTalon((uint32_t) 1),
rightTalon((uint32_t) 1),
gyro((uint32_t) 1),

leftDriveEncoder((uint32_t) 1, (uint32_t) 2),
rightDriveEncoder((uint32_t) 1, (uint32_t) 2),

leftGyroController(1, 1, 1, &gyro, &leftTalon),
rightGyroController(1, 1, 1, &gyro, &rightTalon),
leftDriveController(1, 1, 1, &leftDriveEncoder, &leftTalon),
rightDriveController(1, 1, 1, &rightDriveEncoder, &rightTalon),
state(IDLE)

{


}

void Drivetrain::init() {

}

void Drivetrain::update(){
	switch(state) {
	case IDLE:
		break;
	case TELEOP:
		break;
	case AUTOMATED_DRIVE:
		break;
	case AUTOMATED_ROTATE:
		break;
	case BRAKING:
		break;
	case STOPPED:
		break;
	}
}

void Drivetrain::disable(){

}

void Drivetrain::idle() {
	state = IDLE;
	leftDriveController.Disable();
	rightDriveController.Disable();
	leftGyroController.Disable();
	rightGyroController.Disable();
}

bool Drivetrain::isIdle() {
	return state == IDLE;
}

void Drivetrain::setState(State state) {
	if(state != STOPPED) {
		this -> state = state;
	}
}

bool Drivetrain::encodersStopped() {
	return leftDriveEncoder.GetStopped() && rightDriveEncoder.GetStopped();
}

int Drivetrain::driveControllerError() {
	return std::max(leftDriveController.GetError(), rightDriveController.GetError());
}

int Drivetrain::rotateControllerError() {
	return std::max(leftGyroController.GetError(), rightGyroController.GetError());
}

void Drivetrain::drive() {
	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);
}

Drivetrain::~Drivetrain() {

}

