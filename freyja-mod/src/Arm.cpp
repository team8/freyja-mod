#include "Arm.h"

Arm::Arm() :
	solenoid((uint32_t) PORT_ARM_SOLENOID_A, (uint32_t) PORT_ARM_SOLENOID_B), timer() {
	state = IDLE_OPEN;
}

void Arm::init() {
	setState(IDLE_OPEN);
}

void Arm::update() {
	switch (state) {
	case IDLE_OPEN:
		break;
	case IDLE_CLOSED:
		break;
	case OPENING:
		solenoid.Set(DoubleSolenoid::Value::kReverse);
		if (timer.Get() >= ARM_TRANSITION_TIME) {
			idle();
			setState(IDLE_OPEN);
		}
		break;
	case CLOSING:
		solenoid.Set(DoubleSolenoid::Value::kForward);
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
		break;
	case IDLE_CLOSED:
		open();
		break;
	case OPENING:
		close();
		break;
	case CLOSING:
		open();
		break;
	case DISABLED:
		break;
	default:
		break;
	}
}

void Arm::open() {
	if (state != OPENING && state != IDLE_OPEN) {
		setState(OPENING);
		timer.Reset();
		timer.Start();
	}
}

void Arm::close() {
	if (state != CLOSING && state != IDLE_CLOSED) {
		setState(CLOSING);
		timer.Reset();
		timer.Start();
	}
}

Arm::~Arm() {

}
