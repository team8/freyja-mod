#include <Drivetrain.h>

Drivetrain::Drivetrain() :
	leftTalon1((uint32_t) PORT_DRIVETRAIN_TALON_LEFT_1),
	leftTalon2((uint32_t) PORT_DRIVETRAIN_TALON_LEFT_2),
	rightTalon1((uint32_t) PORT_DRIVETRAIN_TALON_RIGHT_1),
	rightTalon2((uint32_t) PORT_DRIVETRAIN_TALON_RIGHT_2),
	gyro((int32_t) PORT_GYROSCOPE),

	leftEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_LEFT_B, false),
	rightEncoder((uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_A, (uint32_t) PORT_DRIVETRAIN_ENCODER_RIGHT_B, false),

	leftDriveController1(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &leftEncoder, &leftTalon1),
	leftDriveController2(DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &leftEncoder, &leftTalon2),
	rightDriveController1(-DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &rightEncoder, &rightTalon1),
	rightDriveController2(-DRIVE_PROPORTIONAL, DRIVE_INTEGRAL, DRIVE_DERIVATIVE, &rightEncoder, &rightTalon2),
	leftGyroController1(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon1),
	leftGyroController2(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon2),
	rightGyroController1(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon1),
	rightGyroController2(GYRO_PROPORTIONAL, GYRO_INTEGRAL, GYRO_DERIVATIVE, &gyro, &leftTalon2),
	state(IDLE)
{
	std::cout << "Drivetrain constructor called" << std::endl;
	leftEncoder.SetDistancePerPulse(LEFT_DPP);
	rightEncoder.SetDistancePerPulse(RIGHT_DPP);
	leftEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);
	rightEncoder.SetMaxPeriod(ENCODER_MAX_PERIOD);

	leftDriveController1.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	rightDriveController1.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	leftDriveController2.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);
	rightDriveController2.SetInputRange(-ENCODER_INPUT_RANGE, ENCODER_INPUT_RANGE);

	leftDriveController1.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	rightDriveController1.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	leftDriveController2.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);
	rightDriveController2.SetOutputRange(-ENCODER_DRIVE_OUTPUT_RANGE, ENCODER_DRIVE_OUTPUT_RANGE);

	leftGyroController1.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	rightGyroController1.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	leftGyroController2.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
	rightGyroController2.SetOutputRange(-ENCODER_GYRO_OUTPUT_RANGE, ENCODER_GYRO_OUTPUT_RANGE);
}

void Drivetrain::init() {
	state = TELEOP;
	leftEncoder.Reset();
	rightEncoder.Reset();
	gyro.InitGyro();
}

void Drivetrain::update() {
	debug();
	sampleVelocities();
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

	double leftPower = -(scaledForward - scaledTurn);
	double rightPower = scaledForward + scaledTurn;

/*	if(!encodersOnline()) {*/
		simpleDrive(leftPower, rightPower);
/*	}
	else {
		smartDrive(leftPower, rightPower);
	}*/
}

void Drivetrain::simpleDrive(double leftPower, double rightPower) {
	leftTalon1.Set(leftPower);
	leftTalon2.Set(leftPower);
	rightTalon1.Set(rightPower);
	rightTalon2.Set(rightPower);
}

void Drivetrain::smartDrive(int leftPower, int rightPower) {
	//implement me
	//if power > current power + MAXIMUM_POWER_CHANGE or power < currentpower - MAXIMUM_POWER_CHANGE
	//	power = current power + MAXIMUM_POWER_CHANGE or power = current power - MAXIMUM_POWER_CHANGE
	leftTalon1.Set(limitPower(leftPower, prevLeftPower));
	leftTalon2.Set(limitPower(leftPower, prevLeftPower));
	rightTalon1.Set(limitPower(rightPower, prevRightPower));
	rightTalon2.Set(limitPower(rightPower, prevRightPower));
}

void Drivetrain::driveDist(double distance) {
	if(encodersOnline()) {
		setState(AUTOMATED_DRIVE);

		disableGyroControllers();

		leftEncoder.SetPIDSourceParameter(PIDSource::kDistance);
		rightEncoder.SetPIDSourceParameter(PIDSource::kDistance);

		leftDriveController1.SetSetpoint(distance);
		leftDriveController2.SetSetpoint(distance);
		rightDriveController1.SetSetpoint(distance);
		rightDriveController2.SetSetpoint(distance);

		enableDriveControllers();
	}
}

void Drivetrain::rotateAngle(double angle) {
	if(encodersOnline()) {
		setState(AUTOMATED_ROTATE);

		disableDriveControllers();

		leftEncoder.SetPIDSourceParameter(PIDSource::kDistance);
		rightEncoder.SetPIDSourceParameter(PIDSource::kDistance);

		leftGyroController1.SetSetpoint(angle);
		leftGyroController2.SetSetpoint(angle);
		rightGyroController1.SetSetpoint(angle);
		rightGyroController2.SetSetpoint(angle);

		enableGyroControllers();
	}
}

void Drivetrain::brake() {
	if(encodersOnline()) {
		setState(BRAKING);

		disableGyroControllers();

		leftEncoder.SetPIDSourceParameter(PIDSource::kRate);
		rightEncoder.SetPIDSourceParameter(PIDSource::kRate);

		leftDriveController1.SetSetpoint(0);
		leftDriveController2.SetSetpoint(0);
		rightDriveController1.SetSetpoint(0);
		rightDriveController2.SetSetpoint(0);

		enableDriveControllers();
	}
}

void Drivetrain::setState(State state) {
	if(state != STOPPED) {
		this -> state = state;
	}
}

int Drivetrain::limitPower(int power, int prevPower) {
	if(power > prevPower + MAXIMUM_POWER_CHANGE) {
		return prevPower + MAXIMUM_POWER_CHANGE;
	}
	if(power < prevPower - MAXIMUM_POWER_CHANGE) {
		return prevPower - MAXIMUM_POWER_CHANGE;
	}
	return power;
}

double Drivetrain::getLeftAcceleration() {
	return leftVelSum - prevLeftVelSum;
}

double Drivetrain::getRightAcceleration() {
	return rightVelSum - prevRightVelSum;
}

void Drivetrain::sampleVelocities() {
	if(sampleTick == CYCLES_PER_SAMPLE) {
			nextLeftVelSum += leftEncoder.GetRate();
			nextRightVelSum += leftEncoder.GetRate();

			if(sumTick == SAMPLES_PER_CALC) {
				prevLeftVelSum = leftVelSum;
				prevRightVelSum = rightVelSum;

				leftVelSum = nextLeftVelSum/SAMPLES_PER_CALC;
				rightVelSum = nextRightVelSum/SAMPLES_PER_CALC;

				nextLeftVelSum = 0;
				nextRightVelSum = 0;

				sumTick = 0;
			}

			sumTick++;
			sampleTick = 0;
		}
		sampleTick++;
}

bool Drivetrain::isLeftAccelBelowLim() {
	return std::abs(getLeftAcceleration()) < MAXIMUM_POWER_CHANGE;
}

bool Drivetrain::isRightAccelBelowLim() {
	return std::abs(getRightAcceleration()) < MAXIMUM_POWER_CHANGE;
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

bool Drivetrain::encodersOnline() {
	bool retVal = true;
	if(leftEncoder.StatusIsFatal()) {
		std::cout << "WARNING: LEFT ENCODER FATAL - PID, Brakes and SmartDrive Disabled" << std::endl;
		retVal = false;
	}
	if(rightEncoder.StatusIsFatal()) {
		std::cout << "WARNING: RIGHT ENCODER FATAL - PID, Brakes and SmartDrive Disabled" << std::endl;
		retVal = false;
	}
	return retVal;
}

bool Drivetrain::gyroOnline(){
	bool retval = true;
	if(gyro.StatusIsFatal()){
		std::cout << "Gyro Fatal" << std::endl;
		retval = false;
	}
	return retval;
}

void Drivetrain::debug() {
	std::cout << "Drivetrain State: " << state << std::endl;
	std::cout << "Left Encoder  | Raw: " << leftEncoder.GetRaw() << " | Distance: " << leftEncoder.GetDistance()
			<< " | Rate: " << leftEncoder.GetRate() << " | Stopped: " << leftEncoder.GetStopped() << std::endl;
	std::cout << "Right Encoder | Raw: " << rightEncoder.GetRaw() << " | Distance: " << rightEncoder.GetDistance()
			<< " | Rate: " << rightEncoder.GetRate() << " | Stopped: " << rightEncoder.GetStopped() << std::endl;
/*	std::cout << "Gyro 			| Angle: " << gyro.GetAngle() << " | Rate " << gyro.GetRate() << std::endl;
	std::cout << "Left Talon 	| Get: " << leftTalon1.Get() << " | Raw " << leftTalon1.GetRaw() << std::endl;
	std::cout << "Right Talon 	| Get: " << leftTalon1.Get() << " | Raw " << leftTalon1.GetRaw() << std::endl;
	std::cout << "Left Drive Controller | Enabled: " << leftDriveController1.IsEnabled() << " | Setpoint: " << leftDriveController1.GetSetpoint()
		 << " | Error: " << leftDriveController1.GetError() << std::endl;
	std::cout << "Right Drive Controller | Enabled: " << rightDriveController1.IsEnabled() << " | Setpoint: " << rightDriveController1.GetSetpoint()
		 << " | Error: " << rightDriveController1.GetError() << std::endl;
	std::cout << "Left Gyro Controller | Enabled: " << leftGyroController1.IsEnabled() << " | Setpoint: " << leftGyroController1.GetSetpoint()
		 << " | Error: " << leftGyroController1.GetError() << std::endl;
	std::cout << "Right Gyro Controller | Enabled: " << rightGyroController1.IsEnabled() << " | Setpoint: " << rightGyroController1.GetSetpoint()
		 << " | Error: " << rightGyroController1.GetError() << std::endl; */
	std::cout << "--------------------------------------------------------------------------------------------------" << std::endl << std::endl;
}

Drivetrain::~Drivetrain() {

}
