#include <Drivetrain.h>

Drivetrain::Drivetrain() :
leftTalon((uint32_t) PORT_DRIVETRAIN_TALON_LEFT),
rightTalon((uint32_t) PORT_DRIVETRAIN_TALON_RIGHT),
gyro((int32_t) PORT_GYROSCOPE),

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
	gyro.InitGyro();
}

void Drivetrain::update(){
	switch(state) {
	case IDLE:
		break;
	case TELEOP:
		break;
	case AUTOMATED_DRIVE:
		if(encodersStopped() && driveControllerError() < ACCEPTABLE_DRIVE_ERROR) {
			disableControllers();
			setState(IDLE);
		}
		break;
	case AUTOMATED_ROTATE:
		if(encodersStopped() && rotateControllerError() < ACCEPTABLE_ROTATE_ERROR) {
			disableControllers();
			setState(IDLE);
		}
		break;
	case BRAKING:
		if(encodersStopped() && driveControllerError() < ACCEPTABLE_DRIVE_ERROR) {
			disableControllers();
			setState(IDLE);
		}
		break;
	case STOPPED:
		break;
	}
}

void Drivetrain::disable(){
	disableControllers();
	setState(STOPPED);
}

void Drivetrain::idle() {
	disableControllers();
	setState(IDLE);
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

	disableControllers();

	double scaledForward = std::max(std::min(SPEED_CONSTANT * forwardValue, MAX_FORWARD_SPEED), -MAX_FORWARD_SPEED);
	double scaledTurn = std::max(std::min(TURN_CONSTANT * turnValue, MAX_FORWARD_SPEED), -MAX_FORWARD_SPEED);

	leftTalon.Set(-(scaledForward + scaledTurn));
	rightTalon.Set(scaledForward - scaledTurn);
}

void Drivetrain::driveDist(double distance) {
	setState(AUTOMATED_DRIVE);

	disableGyroControllers();

	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftDriveController.SetSetpoint(distance);
	rightDriveController.SetSetpoint(distance);

	enableDriveControllers();
}

void Drivetrain::rotateAngle(double angle) {
	setState(AUTOMATED_ROTATE);

	disableDriveControllers();

	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftGyroController.SetSetpoint(angle);
	rightGyroController.SetSetpoint(angle);

	enableGyroControllers();
}

void Drivetrain::brake() {
	setState(BRAKING);

	disableGyroControllers();

	leftDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);
	rightDriveEncoder.SetPIDSourceParameter(PIDSource::kRate);

	leftDriveController.SetSetpoint(0);
	rightDriveController.SetSetpoint(0);

	enableDriveControllers();
}

void Drivetrain::disableControllers() {
	disableDriveControllers();
	disableGyroControllers();
}

void Drivetrain::enableDriveControllers() {
	leftDriveController.Enable();
	rightDriveController.Enable();
}

void Drivetrain::enableGyroControllers() {
	leftGyroController.Enable();
	rightGyroController.Enable();
}

void Drivetrain::disableDriveControllers() {
	leftDriveController.Disable();
	rightDriveController.Disable();
}

void Drivetrain::disableGyroControllers() {
	leftGyroController.Disable();
	rightGyroController.Disable();
}

Drivetrain::~Drivetrain() {

}
