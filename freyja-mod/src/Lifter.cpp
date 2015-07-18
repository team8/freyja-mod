// Author: Nihar
#include "Lifter.h"

Lifter::Lifter() :
	talon((uint32_t) PORT_LIFTER_TALON_1),
	encoder((uint32_t) PORT_LIFTER_ENCODER_A, (uint32_t) PORT_LIFTER_ENCODER_B),

	controller(PROPORTIONAL_CONSTANT, INTEGRAL_CONSTANT, DERIVATIVE_CONSTANT, &encoder, &talon),

	topSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_TOP),
	bottomSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_BOTTOM),

	currentLevel(0),
	state(IDLE)
{
	controller.SetOutputRange(-MAX_SPEED, MAX_SPEED);
	controller.SetInputRange(-INPUT_RANGE, INPUT_RANGE);
	encoder.SetDistancePerPulse(ENCODER_DPP);
	encoder.SetMaxPeriod(ENCODER_MAX_PERIOD);
}

void Lifter::init() {
	resetZero();
}

void Lifter::update() {
	//Finds current level based on encoder value
	currentLevel = encoder.GetDistance() / LEVEL_HEIGHT;

	switch(state) {
	case IDLE:
		break;
	case TELEOP:
		break;
	case AUTOMATED:
		if(encoder.GetStopped() && controller.GetError() < ACCEPTABLE_PID_ERROR) {
			idle();
		}
		break;
	case DISABLED:
		break;
	}

	// Sets the velocity to the bounce speed if a limit switch is triggerd
	if(isBottomHit()) {
		setVelocity(BOUNCE_SPEED);
	}
	else if(isTopHit()) {
		setVelocity(-BOUNCE_SPEED);
	}
}


void Lifter::disable() {
	controller.Disable();
	setState(DISABLED);
}

void Lifter::idle() {
	encoder.SetPIDSourceParameter(PIDSource::kRate);
	controller.SetSetpoint(0);
	controller.Enable();
	setState(IDLE);
}

bool Lifter::isIdle() {
	return (state == IDLE);
}

void Lifter::setVelocity(double velocity) {
	velocity *= SPEED_SCALING;
	velocity = std::min(std::max(velocity, -MAX_SPEED), MAX_SPEED);
	talon.Set(velocity);
	setState(TELEOP);
}

void Lifter::setLevel(double level) {
	double setpoint = level * LEVEL_HEIGHT;
	encoder.SetPIDSourceParameter(PIDSource::kDistance);
	controller.SetSetpoint(setpoint);
	controller.Enable();
	setState(AUTOMATED);
}

void Lifter::liftLevel(double liftAmount) {
	double newLevel = currentLevel + liftAmount;
	setLevel(newLevel);
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

bool Lifter::isBottomHit() {
	return bottomSensor.Get();
}

bool Lifter::isTopHit() {
	return topSensor.Get();
}
