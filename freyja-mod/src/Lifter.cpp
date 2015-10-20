// Author: Nihar
#include "Lifter.h"

Lifter::Lifter() :
	victor1((uint32_t) PORT_LIFTER_VICTOR_1),
	victor2((uint32_t) PORT_LIFTER_VICTOR_2),
	encoder((uint32_t) PORT_LIFTER_ENCODER_A, (uint32_t) PORT_LIFTER_ENCODER_B, true),

	distanceController1(PROPORTIONAL_CONSTANT, INTEGRAL_CONSTANT, DERIVATIVE_CONSTANT, &encoder, &victor1),
	distanceController2(PROPORTIONAL_CONSTANT, INTEGRAL_CONSTANT, DERIVATIVE_CONSTANT, &encoder, &victor2),
	lifterAccel(),
//	topSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_TOP),
//	bottomSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_BOTTOM),
	topSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_ANALOG_TOP),
	bottomSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_ANALOG_BOTTOM),

	currentLevel(0),
	state(IDLE)
{
	distanceController1.SetOutputRange(-0.7, 0.7);
	distanceController1.SetInputRange(-INPUT_RANGE, INPUT_RANGE);
	distanceController2.SetOutputRange(-0.7, 0.7);
	distanceController2.SetInputRange(-INPUT_RANGE, INPUT_RANGE);
	encoder.SetDistancePerPulse(ENCODER_DPP);
	encoder.SetMaxPeriod(ENCODER_MAX_PERIOD);
//	bottomSensor.SetLimitsVoltage(0.25, 3);
//	topSensor.SetLimitsVoltage(0.25, 3);
}

void Lifter::init() {
	encoder.Reset();
	disableControllers();
	resetZero();
	setState(IDLE);
}

void Lifter::update() {


//
//	if(predictedSpeed < 0 && predictedSpeed > victor1.Get()) {
//		std::cout << "ohi" << std::endl;
//	}
//
//	//Makes sure robot doesn't flip
//	encoder.SetPIDSourceParameter(PIDSource::kRate);
//
//	if(lifterAccel.GetY() - 1 > 0.2 && encoder.Get() < 0) {
////		setVelocity(0.2);
//		std::cout << "STOPPED" << std::endl;
//	}
//	encoder.SetPIDSourceParameter(PIDSource::kDistance);

	debug();

	//Finds current level based on encoder value
	currentLevel = encoder.GetDistance() / LEVEL_HEIGHT;

	switch(state) {
	case IDLE:
		break;
	case TELEOP:
		break;
	case AUTOMATED:
		if(encoder.GetStopped() && distanceController1.GetError() < ACCEPTABLE_PID_ERROR && distanceController2.GetError() < ACCEPTABLE_PID_ERROR) {
			disableControllers();
			setState(TELEOP);
		}
		break;
	case DISABLED:
		break;
	}

	// Sets the velocity to the bounce speed if a limit switch is triggerd
	if(isBottomHit()) {
		//setVelocity(BOUNCE_SPEED);
	}
	else if(isTopHit()) {
		//setVelocity(-BOUNCE_SPEED);
	}
}


void Lifter::disable() {
	distanceController1.Disable();
	distanceController2.Disable();
	encoder.Reset();
	setState(DISABLED);
}

void Lifter::idle() {
	encoder.SetPIDSourceParameter(PIDSource::kRate);
	distanceController1.SetSetpoint(0);
	distanceController2.SetSetpoint(0);
	distanceController1.Enable();
	distanceController2.Enable();
	setState(IDLE);
}

bool Lifter::isIdle() {
	return (state == IDLE);
}

void Lifter::setVelocity(double velocity) {
	//std::cout << "velocity: " << velocity << std::endl;
	double computedVelocity = std::min(std::max(velocity * SPEED_SCALING, -MAX_SPEED), MAX_SPEED);
	predictedSpeed = computedVelocity;
	//std::cout << "computedVelocity: " << computedVelocity << std::endl;
	victor1.Set(computedVelocity);
	victor2.Set(computedVelocity);
	setState(TELEOP);
}

void Lifter::disableControllers() {
	distanceController1.Disable();
	distanceController2.Disable();
}

void Lifter::setLevel(double level) {
	double setpoint = level * LEVEL_HEIGHT;
	encoder.SetPIDSourceParameter(PIDSource::kDistance);
	distanceController1.SetSetpoint(setpoint);
	distanceController2.SetSetpoint(setpoint);
	distanceController1.Enable();
	distanceController2.Enable();
	setState(AUTOMATED);
}

void Lifter::liftLevel(double liftAmount) {
	double newLevel = currentLevel + liftAmount;
	setLevel(newLevel);
}

void Lifter::levelUp() {
	setLevel((int) (currentLevel + 1));
}

void Lifter::levelDown() {
	setLevel((int) currentLevel);
}

void Lifter::zero() {
	setLevel(0);
}

void Lifter::resetZero() {
	encoder.Reset();
}

void Lifter::setState(State state) {
	if(state == DISABLED) {
		return;
	}
	this -> state = state;
}

void Lifter::debug() {
	std::cout << "Lifter State: " << state << std::endl;
	std::cout << "Encoder  | Raw: " << encoder.GetRaw() << " | Distance: " << encoder.GetDistance() << " | Rate: " << encoder.GetRate() << " | Stopped: " << encoder.GetStopped() << std::endl;
//	std::cout << "Victor 	| Get: " << victor1.Get() << " | Raw " << victor1.GetRaw() << std::endl;
//	std::cout << "Controller | Enabled: " <<  controller1.IsEnabled() << " | Setpoint: " << controller1.GetSetpoint()<< " | Error: " << controller1.GetError() << std::endl;
	//std::cout << "Hall Effect Sensor | Bottom: " << bottomSensor.Get() << " | Top: " << topSensor.Get() << std::endl;
//	std::cout << "X-Axis   " << lifterAccel.GetX() << "\n";
//	std::cout << "Y-Axis   " << lifterAccel.GetY() -1 << "\n";
//	std::cout << "Z-Axis   " << lifterAccel.GetZ() << "\n";
//	std::cout << "--------------------- " << std::endl;
	std::cout << "Controller error" << distanceController1.GetError() <<std::endl;
}

bool Lifter::isBottomHit() {
	return bottomSensor.Get();
}

bool Lifter::isTopHit() {
	return topSensor.Get();
}
