#include <Drivetrain.h>

Drivetrain::Drivetrain() :
	leftTalon1((uint32_t) PORT_DRIVETRAIN_TALON_LEFT_1),
	leftTalon2((uint32_t) PORT_DRIVETRAIN_TALON_LEFT_2),
	rightTalon1((uint32_t) PORT_DRIVETRAIN_TALON_RIGHT_1),
	rightTalon2((uint32_t) PORT_DRIVETRAIN_TALON_RIGHT_2),
	gyro((int32_t) PORT_GYROSCOPE),

	leftEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_B, true),
	rightEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_B, false),

	leftGyroController1(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon1),
	rightGyroController1(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &rightTalon1),
	leftDriveController1(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &leftEncoder, &leftTalon1),
	rightDriveController1(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &rightEncoder, &rightTalon1),
	leftGyroController2(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon2),
	rightGyroController2(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &rightTalon2),
	leftDriveController2(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &leftEncoder, &leftTalon2),
	rightDriveController2(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &rightEncoder, &rightTalon2),
	state(IDLE)
{
	leftEncoder.SetDistancePerPulse(LEFT_DPP);
	rightEncoder.SetDistancePerPulse(RIGHT_DPP);
	leftEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);
	rightEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);

	leftDriveController1.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	rightDriveController1.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	leftDriveController1.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	rightDriveController1.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	leftGyroController1.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	rightGyroController1.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	leftDriveController2.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	rightDriveController2.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	leftDriveController2.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	rightDriveController2.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	leftGyroController2.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	rightGyroController2.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
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

	leftTalon1.Set(-(scaledForward + scaledTurn));
	leftTalon2.Set(-(scaledForward + scaledTurn));
	rightTalon1.Set(scaledForward - scaledTurn);
	leftTalon2.Set(-(scaledForward + scaledTurn));
}

void Drivetrain::driveDist(double distance) {
	setState(AUTOMATED_DRIVE);

	disableGyroControllers();

	leftEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftDriveController1.SetSetpoint(distance);
	leftDriveController1.SetSetpoint(distance);
	rightDriveController2.SetSetpoint(distance);
	rightDriveController2.SetSetpoint(distance);

	enableDriveControllers();
}

void Drivetrain::rotateAngle(double angle) {
	setState(AUTOMATED_ROTATE);

	disableDriveControllers();

	leftEncoder.SetPIDSourceParameter(PIDSource::kDistance);
	rightEncoder.SetPIDSourceParameter(PIDSource::kDistance);

	leftGyroController1.SetSetpoint(angle);
	leftGyroController1.SetSetpoint(angle);
	rightGyroController2.SetSetpoint(angle);
	rightGyroController2.SetSetpoint(angle);

	enableGyroControllers();
}

void Drivetrain::brake() {
	setState(BRAKING);

	disableGyroControllers();

	leftEncoder.SetPIDSourceParameter(PIDSource::kRate);
	rightEncoder.SetPIDSourceParameter(PIDSource::kRate);

	leftDriveController1.SetSetpoint(0);
	leftDriveController1.SetSetpoint(0);
	rightDriveController2.SetSetpoint(0);
	rightDriveController2.SetSetpoint(0);

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
	return std::max(std::max(leftDriveController1.GetError(), leftDriveController2.GetError()),
			std::max(rightDriveController1.GetError(), rightDriveController2.GetError()));
}

int Drivetrain::rotateControllerError() {
	return std::max(std::max(leftGyroController1.GetError(), leftGyroController2.GetError()),
			std::max(rightGyroController1.GetError(), rightGyroController2.GetError()));
}

void Drivetrain::enableGyroControllers() {
	leftGyroController1.Enable();
	leftGyroController2.Enable();
	rightGyroController1.Enable();
	rightGyroController2.Enable();
}

void Drivetrain::enableDriveControllers() {
	leftDriveController1.Enable();
	leftDriveController2.Enable();
	rightDriveController1.Enable();
	rightDriveController2.Enable();
}


void Drivetrain::disableControllers() {
	disableDriveControllers();
	disableGyroControllers();
}

void Drivetrain::disableGyroControllers() {
	leftGyroController1.Disable();
	leftGyroController2.Disable();
	rightGyroController1.Disable();
	rightGyroController2.Disable();
}

void Drivetrain::disableDriveControllers() {
	leftDriveController1.Disable();
	leftDriveController2.Disable();
	rightDriveController1.Disable();
	rightDriveController2.Disable();
}

void Drivetrain::debug() {
	std::cout << "Drivetrain State: " << state << std::endl;
	std::cout << "Left Encoder  | Raw: " << leftEncoder.GetRaw() << " | Distance: " << leftEncoder.GetDistance()
			<< " | Rate: " << leftEncoder.GetRate() << " | Stopped: " << leftEncoder.GetStopped() << std::endl;
	std::cout << "Right Encoder | Raw: " << rightEncoder.GetRaw() << " | Distance: " << rightEncoder.GetDistance()
			<< " | Rate: " << rightEncoder.GetRate() << " | Stopped: " << rightEncoder.GetStopped() << std::endl;
	std::cout << "Gyro 			| Angle: " << gyro.GetAngle() << " | Rate " << gyro.GetRate() << std::endl;
	std::cout << "Left Talon 	| Get: " << leftTalon1.Get() << " | Raw " << leftTalon1.GetRaw() << std::endl;
	std::cout << "Right Talon 	| Get: " << leftTalon1.Get() << " | Raw " << leftTalon1.GetRaw() << std::endl;
	std::cout << "Left Drive Controller | Enabled: " << leftDriveController1.IsEnabled() << " | Setpoint: " << leftDriveController1.GetSetpoint()
		 << " | Error: " << leftDriveController1.GetError() << std::endl;
	std::cout << "Right Drive Controller | Enabled: " << rightDriveController1.IsEnabled() << " | Setpoint: " << rightDriveController1.GetSetpoint()
		 << " | Error: " << rightDriveController1.GetError() << std::endl;
	std::cout << "Left Gyro Controller | Enabled: " << leftGyroController1.IsEnabled() << " | Setpoint: " << leftGyroController1.GetSetpoint()
		 << " | Error: " << leftGyroController1.GetError() << std::endl;
	std::cout << "Right Gyro Controller | Enabled: " << rightGyroController1.IsEnabled() << " | Setpoint: " << rightGyroController1.GetSetpoint()
		 << " | Error: " << rightGyroController1.GetError() << std::endl;
	std::cout << "--------------------------------------------------------------------------------------------------" << std::endl << std::endl;
}

Drivetrain::~Drivetrain() {

}
