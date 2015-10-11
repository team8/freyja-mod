/*
 * Ramp.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: Freddy
 */
#include "Ramp.h"
#include "Ports.h"

/**
 * Default constructor
 * Sets victor ports
 */
Ramp::Ramp():
	rightVic((uint32_t) PORT_RAMP_RIGHT_VIC),
	leftVic((uint32_t) PORT_RAMP_LEFT_VIC),
	deployTimer()
{
	state = State::IDLE;
	rampToggled = false;
	rightVic.Set(0);
	leftVic.Set(0);
}

/**
 * Initializes the ramp and its components
 */
void Ramp::init() {
	state = State::IDLE;
	rightVic.Set(0);
	leftVic.Set(0);
}

/**
 * Updates the ramp and its components
 */
void Ramp::update() {
	switch(state) {
	case(State::RUNNING):
		rightVic.Set(-RIGHT_SPEED);
		leftVic.Set(LEFT_SPEED);
		break;
	case(State::SLOWING):
		rightVic.Set(RIGHT_SPEED);
		leftVic.Set(-LEFT_SPEED);
		break;
	case(State::IDLE):
		rightVic.Set(0);
		leftVic.Set(0);
		break;
	case(State::DEPLOYING):
		rightVic.Set(-RIGHT_SPEED);
		leftVic.Set(LEFT_SPEED);
		if(deployTimer.Get() > DEPLOY_TIME) {
			setState(IDLE);
			deployTimer.Stop();
			deployTimer.Reset();
		}
		break;
	}
}

/**
 * Disables the ramp and its components
 */
void Ramp::disable() {
	state = State::IDLE;
	rightVic.Set(0);
	leftVic.Set(0);
}

/**
 * Toggles the ramp state
 */
void Ramp::toggleRampDeploy() {
	if(!rampToggled) {
		if(state != State::IDLE) {
			stop();
		}
		else {
			deploy();
		}
		rampToggled = true;
	}
}

/**
 * Toggles the ramp state
 */
void Ramp::toggleRampSlow() {
	if(!rampToggled) {
		if(state != State::IDLE) {
			stop();
		}
		else {
			slow();
		}
		rampToggled = true;
	}
}

/**
 * Resets the toggle state
 */
void Ramp::toggleRampReset() {
	rampToggled = false;
}

/**
 * Starts turning the wheels and manages their spin
 */
void Ramp::start() {
	setState(RUNNING);
}

/**
 * Starts turning the wheels and manages their spin
 */
void Ramp::slow() {
	setState(SLOWING);
}

/**
 * Starts turning the wheels and manages their spin
 */
void Ramp::deploy() {
	setState(DEPLOYING);
	deployTimer.Reset();
	deployTimer.Start();
}

/**
 * Stops the turning of the wheels and
 */
void Ramp::stop() {
	setState(IDLE);
}

/**
 * Sets the state of the ramp
 */
void Ramp::setState(State state) {
	this -> state = state;
}

/**
 * Idles the ramp
 */
void Ramp::idle() {
	stop();
}

/**
 * Returns whether or not the ramp is idle
 */
bool Ramp::isIdle() {
	return (state == Ramp::State::IDLE);
}

