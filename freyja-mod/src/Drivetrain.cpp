#include <Drivetrain.h>

Drivetrain::Drivetrain() :
	leftTalon((uint32_t) PORT_DRIVETRAIN_TALON_LEFT),
	rightTalon((uint32_t) PORT_DRIVETRAIN_TALON_RIGHT),
	gyro((int32_t) PORT_GYROSCOPE),

	leftEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_B, true),
	rightEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_B, false),

	leftGyroController(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon),
	rightGyroController(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &rightTalon),
	leftDriveController(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &leftEncoder, &leftTalon),
	rightDriveController(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &rightEncoder, &rightTalon),
	state(IDLE)

{
	leftEncoder.SetDistancePerPulse(LEFT_DPP);
	rightEncoder.SetDistancePerPulse(RIGHT_DPP);
	leftEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);
	rightEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);

	leftDriveController.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	rightDriveController.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	leftDriveController.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	rightDriveController.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	leftGyroController.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	rightGyroController.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
}

void Drivetrain::init() {
	leftEncoder.Reset();
	rightEncoder.Reset();
	gyro.InitGyro();
}

void Drivetrain::update() {
	debug();
	switch(state) {
	case IDLE:
		break;
	case TELEOP:
		break;
	case AUTOMATED_DRIVE:
		if(encodersStopped() && driveControllerError() < ACCEPTABLE_DRIVE_ERROR) {
			idle();
		}
		break;
	case AUTOMATED_ROTATE:
		if(encodersStopped() && rotateControllerError() < ACCEPTABLE_ROTATE_ERROR) {
			idle();
		}
		break;
	case BRAKING:
		if(encodersStopped() && driveControllerError() < ACCEPTABLE_BRAKE_ERROR) {
			idle();
		}
		break;
	case STOPPED:
		break;
	}
}

void Drivetrain::disable() {
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


void Drivetrain::drive(double turnValue, double forwardValue) {
	setState(TELEOP);

	disableControllers();

	double scaledForward = std::max(std::min(SPEED_SCALING * forwardValue, MAX_FORWARD_SPEED), -MAX_FORWARD_SPEED);
	double scaledTurn = std::max(std::min(TURN_SCALING * turnValue, MAX_TURN_SPEED), -MAX_TURN_SPEED);

	leftTalon.Set(-(scaledForward + scaledTurn));
	rightTalon.Set(scaledForward - scaledTurn);
}

void Drivetrain::driveDist(double distance) {
	setState(AUTOMATED_DRIVE);

	disableGyroControllers();

	leftEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftDriveController.SetSetpoint(distance);
	rightDriveController.SetSetpoint(distance);

	enableDriveControllers();
}

void Drivetrain::rotateAngle(double angle) {
	setState(AUTOMATED_ROTATE);

	disableDriveControllers();

	leftEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftGyroController.SetSetpoint(angle);
	rightGyroController.SetSetpoint(angle);

	enableGyroControllers();
}

void Drivetrain::brake() {
	setState(BRAKING);

	disableGyroControllers();

	leftEncoder.SetPIDSourceParameter(PIDSource::kRate);
	rightEncoder.SetPIDSourceParameter(PIDSource::kRate);

	leftDriveController.SetSetpoint(0);
	rightDriveController.SetSetpoint(0);

	enableDriveControllers();
}

void Drivetrain::setState(State state) {
	if(state != STOPPED) {
		this -> state = state;
	}
}

bool Drivetrain::encodersStopped() {
	return leftEncoder.GetStopped() && rightEncoder.GetStopped();
}

int Drivetrain::driveControllerError() {
	return std::max(leftDriveController.GetError(), rightDriveController.GetError());
}

int Drivetrain::rotateControllerError() {
	return std::max(leftGyroController.GetError(), rightGyroController.GetError());
}

void Drivetrain::enableGyroControllers() {
	leftGyroController.Enable();
	rightGyroController.Enable();
}

void Drivetrain::enableDriveControllers() {
	leftDriveController.Enable();
	rightDriveController.Enable();
}


void Drivetrain::disableControllers() {
	disableDriveControllers();
	disableGyroControllers();
}

void Drivetrain::disableGyroControllers() {
	leftGyroController.Disable();
	rightGyroController.Disable();
}

void Drivetrain::disableDriveControllers() {
	leftDriveController.Disable();
	rightDriveController.Disable();
}

void Drivetrain::debug() {
	std::cout << "Drivetrain State: " << state << std::endl;
	std::cout << "Left Encoder  | Raw: " << leftEncoder.GetRaw() << " | Distance: " << leftEncoder.GetDistance()
			<< " | Rate: " << leftEncoder.GetRate() << " | Stopped: " << leftEncoder.GetStopped() << std::endl;
	std::cout << "Right Encoder | Raw: " << rightEncoder.GetRaw() << " | Distance: " << rightEncoder.GetDistance()
			<< " | Rate: " << rightEncoder.GetRate() << " | Stopped: " << rightEncoder.GetStopped() << std::endl;
	std::cout << "Gyro 			| Angle: " << gyro.GetAngle() << " | Rate " << gyro.GetRate() << std::endl;
	std::cout << "Left Talon 	| Get: " << leftTalon.Get() << " | Raw " << leftTalon.GetRaw() << std::endl;
	std::cout << "Right Talon 	| Get: " << leftTalon.Get() << " | Raw " << leftTalon.GetRaw() << std::endl;
	std::cout << "Left Drive Controller | Enabled: " << leftDriveController.IsEnabled() << " | Setpoint: " << leftDriveController.GetSetpoint()
		 << " | Error: " << leftDriveController.GetError() << std::endl;
	std::cout << "Right Drive Controller | Enabled: " << rightDriveController.IsEnabled() << " | Setpoint: " << rightDriveController.GetSetpoint()
		 << " | Error: " << rightDriveController.GetError() << std::endl;
	std::cout << "Left Gyro Controller | Enabled: " << leftGyroController.IsEnabled() << " | Setpoint: " << leftGyroController.GetSetpoint()
		 << " | Error: " << leftGyroController.GetError() << std::endl;
	std::cout << "Right Gyro Controller | Enabled: " << rightGyroController.IsEnabled() << " | Setpoint: " << rightGyroController.GetSetpoint()
		 << " | Error: " << rightGyroController.GetError() << std::endl;
	std::cout << "--------------------------------------------------------------------------------------------------" << std::endl << std::endl;
}

Drivetrain::~Drivetrain() {

}
