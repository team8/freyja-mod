/*
 * Lifter.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: Nihar
 *  Version 2.0
 */
#include "Lifter.h"

Lifter::Lifter() :
	talon((uint32_t) PORT_LIFTER_TALON_1),
	encoder((uint32_t) PORT_LIFTER_ENCODER_A, (uint32_t) PORT_LIFTER_ENCODER_B),
	controller(PROPORTIONAL, INTEGRAL, DERIVATIVE, &encoder, &talon),
	topSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_TOP),
	bottomSensor((uint32_t) PORT_LIFTER_HALL_EFFECT_BOTTOM),
	state(IDLE),
	currentLevel(0)
{
}

void Lifter::init() {
	//Used to vary sensitivity of lifter's speed
	controller.SetOutputRange(-0.5, 0.5);
	controller.SetInputRange(-9999, 9999);

	//Initialize PID components
	encoder.Reset();
	controller.Reset();
	encoder.SetDistancePerPulse(DPP);
	state = IDLE;
}

/*
 * Constantly updates the subsystem,
 * operating based on state machine
 * Updates currentLevel
 */
void Lifter::update() {
	//Finds current level based on encoder value
	currentLevel = encoder.GetDistance()/LEVEL_HEIGHT;
	switch(state) {
	/*
	 * Runs velocity PID to maintain position
	 */
	case IDLE:
		break;
	/*
	 * Scales joystick input to run talon
	 */
	case TELEOP:
		break;
	/*
	 * Runs positon PID to change level
	 */
	case AUTOMATED:
		if(encoder.GetStopped() && controller.GetError() < ACCEPTABLE_PID_ERROR) {
			idle();
		}
		break;
	/*
	 * Unoperational for testing other things
	 */
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
	//Switches to distance PID
	encoder.SetPIDSourceParameter(PIDSource::kDistance);
	controller.SetSetpoint(setpoint);
	setState(AUTOMATED);
}

/*
 * Resets the lifter's zero point to its current location
 */
void Lifter::zero() {
	encoder.Reset();
	currentLevel = 0;
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
	//Change to and use velocity PID to
	encoder.SetPIDSourceParameter(PIDSource::kRate);
	controller.SetSetpoint(0);
	setState(IDLE);
}

bool Lifter::isIdle() {
	return (state == IDLE);
}
