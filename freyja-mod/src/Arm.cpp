#include "Arm.h"

Arm::Arm() :
	solenoid((uint32_t) PORT_ARM_SOLENOID_A, (uint32_t) PORT_ARM_SOLENOID_B), compressor(), timer() {
	state = IDLE_OPEN;
}

void Arm::init() {
	setState(IDLE_OPEN);
	compressor.Start();
}

void Arm::update() {
	switch (state) {
	case IDLE_OPEN:
		break;
	case IDLE_CLOSED:
		break;
	case OPENING:
		if (timer.Get() >= ARM_TRANSITION_TIME) {
			idle();
			setState(IDLE_OPEN);
		}
		break;
	case CLOSING:
		if (timer.Get() >= ARM_TRANSITION_TIME) {
			idle();
			setState(IDLE_CLOSED);
		}
		break;
	case DISABLED:
		break;
	}
}

void Arm::disable() {
	solenoid.Set(DoubleSolenoid::Value::kOff);
	timer.Stop();
	compressor.Stop();
	setState(DISABLED);
}

void Arm::idle() {
	solenoid.Set(DoubleSolenoid::Value::kOff);
	timer.Stop();
}

bool Arm::isIdle() {
	return (state == IDLE_OPEN || state == IDLE_CLOSED);
}

void Arm::setState(State state) {
	if (state != DISABLED) {
		this -> state = state;
	}
}

void Arm::toggle() {
	switch (state) {
	case IDLE_OPEN:
		close();
	case OPENING:
		close();
		break;
	case IDLE_CLOSED:
		open();
	case CLOSING:
		open();
		break;
	case DISABLED:
		break;
	}
}

void Arm::open() {
	if (state != OPENING && state != IDLE_OPEN) {
		setState(OPENING);
		solenoid.Set(DoubleSolenoid::Value::kReverse);
		timer.Reset();
		timer.Start();
	}
}

void Arm::close() {
	if (state != CLOSING && state != IDLE_CLOSED) {
		setState(CLOSING);
		solenoid.Set(DoubleSolenoid::Value::kForward);
		timer.Reset();
		timer.Start();
	}
}

Arm::~Arm() {

}
