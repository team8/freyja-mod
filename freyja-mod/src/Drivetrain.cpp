#include <Drivetrain.h>

Drivetrain::Drivetrain() :
leftTalon((uint32_t) PORT_DRIVETRAIN_TALON_LEFT),
rightTalon((uint32_t) PORT_DRIVETRAIN_TALON_RIGHT),
gyro((uint32_t) PORT_GYROSCOPE),

leftDriveEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_B, true),
rightDriveEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_B, false),

leftGyroController(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon),
rightGyroController(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &rightTalon),
leftDriveController(LEFT_DRIVE_PROPORTIONAL, LEFT_DRIVE_INTEGRAL, LEFT_DRIVE_DERIVATIVE, &leftDriveEncoder, &leftTalon),
rightDriveController(RIGHT_DRIVE_PROPORTIONAL, RIGHT_DRIVE_INTEGRAL, RIGHT_DRIVE_DERIVATIVE, &rightDriveEncoder, &rightTalon),
state(IDLE)

{
	leftDriveEncoder.SetDistancePerPulse(LEFT_DPP);
	rightDriveEncoder.SetDistancePerPulse(RIGHT_DPP);
	leftDriveEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);
	rightDriveEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);

	leftDriveController.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	rightDriveController.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	leftDriveController.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	rightDriveController.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	leftGyroController.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	rightGyroController.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
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
		if(encodersStopped() && driveControllerError() < 1) {
			setState(IDLE);
		}
		break;
	case AUTOMATED_ROTATE:
		if(encodersStopped() && rotateControllerError() < 1) {
			setState(IDLE);
		}
		break;
	case BRAKING:
		if(encodersStopped() && driveControllerError() < 1) {
			setState(IDLE);
		}
		break;
	case STOPPED:
		break;
	}
}

void Drivetrain::disable(){

}

void Drivetrain::idle() {
	setState(IDLE);
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

void Drivetrain::drive(double turnValue, double forwardValue) {
	setState(TELEOP);

	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);

	int scaledForward = std::max(std::min(SPEED_CONSTANT * forwardValue, MAX_FORWARD_SPEED), -MAX_FORWARD_SPEED);
	int scaledTurn = std::max(std::min(TURN_CONSTANT * turnValue, MAX_FORWARD_SPEED), -MAX_FORWARD_SPEED);


	leftDriveController.SetSetpoint(scaledForward + scaledTurn);
	rightDriveController.SetSetpoint(scaledForward + scaledTurn);
}

void Drivetrain::driveDist(double distance) {
	setState(AUTOMATED_DRIVE);

	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftDriveController.SetSetpoint(distance);
	rightDriveController.SetSetpoint(distance);

	leftDriveController.Enable();
	rightDriveController.Enable();
}

void Drivetrain::rotateAngle(double angle) {
	setState(AUTOMATED_ROTATE);

	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftGyroController.SetSetpoint(angle);
	rightGyroController.SetSetpoint(angle);

	leftGyroController.Enable();
	rightGyroController.Enable();
}

void Drivetrain::brake() {
	setState(BRAKING);

	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);

	leftDriveController.SetSetpoint(0);
	rightDriveController.SetSetpoint(0);
}

Drivetrain::~Drivetrain() {

}
