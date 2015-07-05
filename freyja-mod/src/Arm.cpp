#include "Arm.h"

Arm::Arm() :
	compressor(), solenoid((uint32_t) PORT_ARM_SOLENOID_A, (uint32_t) PORT_ARM_SOLENOID_B), timer() {
	state = IDLE_OPEN;
	compressorState = ON;
}

void Arm::init() {
	compressorState = ON;
	setState(IDLE_OPEN);
}

void Arm::update() {
	switch (state) {
	case IDLE_OPEN:
	case IDLE_CLOSED:
		solenoid.Set(DoubleSolenoid::Value::kOff);
		timer.Stop();
		timer.Reset();
		break;
	case OPENING:
		solenoid.Set(DoubleSolenoid::Value::kReverse);
		if (timer.Get() >= ARM_EXTEND_TIME) {
			setState(IDLE_OPEN);
		}
		break;
	case CLOSING:
		solenoid.Set(DoubleSolenoid::Value::kForward);
		if (timer.Get() >= ARM_EXTEND_TIME) {
			setState(IDLE_CLOSED);
		}
		break;
	case DISABLED:
		compressor.Stop();
		break;
	}

	switch (compressorState) {
	case ON:
		compressor.Start();
		break;
	case OFF:
		compressor.Stop();
		break;
	}
}

void Arm::disable() {
	setState(STOPPED);
}

void Arm::idle() {
	//do  (which idle state to change to?)
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
		open();
		break;
	case IDLE_CLOSED:
		close();
		break;
	case OPENING:
	case CLOSING:
	case DISABLED:
		break;
	default:
		break;
	}
}

void Arm::open() {
	if (state != OPENING) {
		setState(OPENING);
		timer.Start();
	}
}

void Arm::close() {
	if (state != CLOSING) {
		setState(CLOSING);
		timer.Start();
	}
}

Arm::~Arm() {

}
