// Author: Nihar
#include "Lifter.h"

Lifter::Lifter() :
	victor1((uint32_t) PORT_LIFTER_VICTOR_1),
	victor2((uint32_t) PORT_LIFTER_VICTOR_2),
	encoder((uint32_t) PORT_LIFTER_ENCODER_A, (uint32_t) PORT_LIFTER_ENCODER_B, true),

	controller1(PROPORTIONAL_CONSTANT, INTEGRAL_CONSTANT, DERIVATIVE_CONSTANT, &encoder, &victor1),
	controller2(PROPORTIONAL_CONSTANT, INTEGRAL_CONSTANT, DERIVATIVE_CONSTANT, &encoder, &victor2),
//	topSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_TOP),
//	bottomSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_BOTTOM),
	topAnalogSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_ANALOG_TOP),
	bottomAnalogSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_ANALOG_BOTTOM),

	currentLevel(0),
	state(IDLE)
{
	controller1.SetOutputRange(-MAX_SPEED, MAX_SPEED);
	controller1.SetInputRange(-INPUT_RANGE, INPUT_RANGE);
	controller2.SetOutputRange(-MAX_SPEED, MAX_SPEED);
	controller2.SetInputRange(-INPUT_RANGE, INPUT_RANGE);
	encoder.SetDistancePerPulse(ENCODER_DPP);
	encoder.SetMaxPeriod(ENCODER_MAX_PERIOD);
	bottomAnalogSensor.SetLimitsVoltage(0.25, 3);
	topAnalogSensor.SetLimitsVoltage(0.25, 3);
}

void Lifter::init() {
	encoder.Reset();
	controller1.Disable();
	controller2.Disable();
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

//	debug();

	//Finds current level based on encoder value
	currentLevel = encoder.GetDistance() / LEVEL_HEIGHT;

	switch(state) {
	case IDLE:
		break;
	case TELEOP:
		break;
	case AUTOMATED:
		if(encoder.GetStopped() && controller1.GetError() < ACCEPTABLE_PID_ERROR && controller2.GetError() < ACCEPTABLE_PID_ERROR) {
			idle();
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
	controller1.Disable();
	controller2.Disable();
	encoder.Reset();
	setState(DISABLED);
}

void Lifter::idle() {
	encoder.SetPIDSourceParameter(PIDSource::kRate);
	controller1.SetSetpoint(0);
	controller2.SetSetpoint(0);
	controller1.Enable();
	controller2.Enable();
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

void Lifter::setLevel(double level) {
	double setpoint = level * LEVEL_HEIGHT;
	encoder.SetPIDSourceParameter(PIDSource::kDistance);
	controller1.SetSetpoint(setpoint);
	controller2.SetSetpoint(setpoint);
	controller1.Enable();
	controller2.Enable();
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

double Lifter::getVelocity() {
	return encoder.GetRate();
}

void Lifter::setState(State state) {
	if(state == DISABLED) {
		return;
	}
	this -> state = state;
}

void Lifter::debug() {
//	std::cout << "Lifter State: " << state << std::endl;
//	std::cout << "Encoder  | Raw: " << encoder.GetRaw() << " | Distance: " << encoder.GetDistance() << " | Rate: " << encoder.GetRate() << " | Stopped: " << encoder.GetStopped() << std::endl;
//	std::cout << "Victor 	| Get: " << victor1.Get() << " | Raw " << victor1.GetRaw() << std::endl;
//	std::cout << "Controller | Enabled: " <<  controller1.IsEnabled() << " | Setpoint: " << controller1.GetSetpoint()<< " | Error: " << controller1.GetError() << std::endl;
	std::cout << "Hall Effect Sensor | Bottom: " << bottomAnalogSensor.GetTriggerState() << " | Top: " << topAnalogSensor.GetTriggerState() << std::endl;
//	std::cout << "X-Axis   " << lifterAccel.GetX() << "\n";
//	std::cout << "Y-Axis   " << lifterAccel.GetY() -1 << "\n";
//	std::cout << "Z-Axis   " << lifterAccel.GetZ() << "\n";
//	std::cout << "--------------------- " << std::endl;
}

bool Lifter::isBottomHit() {
	return bottomAnalogSensor.GetTriggerState();
}

bool Lifter::isTopHit() {
	return topAnalogSensor.GetTriggerState();
}

bool Lifter::encodersOnline(){
	bool retval = true;
	if(encoder.StatusIsFatal()) {
		std::cout << "WARNING: LIFTER ENCODER FATAL - SmartLift, PID and Idling disabled" << std::endl;
		retval = false;
	}

	return retval;
}
