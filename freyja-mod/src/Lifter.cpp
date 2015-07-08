/*
 * Author: Nihar
 */
#include "Lifter.h"

Lifter::Lifter() :
	talon((uint32_t) PORT_LIFTER_TALON_1),
	encoder((uint32_t) PORT_LIFTER_ENCODER_A, (uint32_t) PORT_LIFTER_ENCODER_B),

	controller(PROPORTIONAL, INTEGRAL, DERIVATIVE, &encoder, &talon),

	topSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_TOP),
	bottomSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_BOTTOM),

	currentLevel(0),
	state(IDLE)
{
	controller.SetOutputRange(-MAX_SPEED, MAX_SPEED);
	controller.SetInputRange(-INPUT_RANGE, INPUT_RANGE);
	encoder.SetDistancePerPulse(DPP);
}

void Lifter::init() {
	encoder.Reset();
	controller.Reset();
}

/*
 * Constantly updates the subsystem,
 * operating based on state machine
 * Updates currentLevel
 */
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

	/*
	 * Moves away from the side in the case of Hall effect sensor trigger
	 */
	if(isBottomHit()) {
		setVelocity(BOUNCE_SPEED);
	}
	else if(isTopHit()) {
		setVelocity(-BOUNCE_SPEED);
	}
}

/*
 * Permanently disables this subsystem for use
 */
void Lifter::disable() {
	controller.Disable();
	talon.Set(0);
	setState(DISABLED);
}

/*
 * Changes the state, can't change from disabled
 * Allows interrupt of automated
 */
void Lifter::setState(State state) {
	if(state == DISABLED) {
		return;
	}
	this -> state = state;
}

/*
 * Passed a target level which is translated
 * into a PID setPoint change
 * Disables velocity PID
 */
void Lifter::setLevel(double level) {
	double setpoint = level * LEVEL_HEIGHT;
	encoder.SetPIDSourceParameter(PIDSource::kDistance);
	controller.SetSetpoint(setpoint);
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

/*
 * Modifies the velocity PID targetVelocity
 * Disables position PID
 */
void Lifter::setVelocity(double velocity) {
	velocity *= SPEED_SCALING;
	velocity = std::min(std::max(velocity, -MAX_SPEED), MAX_SPEED);
	talon.Set(velocity);
	setState(TELEOP);
}

bool Lifter::isBottomHit() {
	return bottomSensor.Get();
}

bool Lifter::isTopHit() {
	return topSensor.Get();
}

void Lifter::idle() {
	encoder.SetPIDSourceParameter(PIDSource::kRate);
	controller.SetSetpoint(0);
	setState(IDLE);
}

bool Lifter::isIdle() {
	return (state == IDLE);
}
