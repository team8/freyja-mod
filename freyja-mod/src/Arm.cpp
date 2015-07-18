#include <Arm.h>

Arm::Arm() : comp(), sol((uint32_t) 0), timer(), state(IDLE_OPEN) {

}

void Arm::init() {
	comp.Start();
}

void Arm::update(){
	switch(state) {
		case IDLE_OPEN:
			sol.Set(DoubleSolenoid::Value::kOff);
			timer.Stop();
			timer.Reset();
			break;
		case IDLE_CLOSED:
			sol.Set(DoubleSolenoid::Value::kOff);
			timer.Stop();
			timer.Reset();
			break;
		case OPENING:
			sol.Set(DoubleSolenoid::Value::kReverse);
			if(timer.Get() >= ARM_EXTEND_TIME) {
				setState(IDLE_OPEN);
			}
			break;
		case CLOSING:
			sol.Set(DoubleSolenoid::Value::kForward);
			if(timer.Get() >= ARM_EXTEND_TIME) {
				setState(IDLE_CLOSED);
			}
			break;
	}

}

void Arm::disable(){
	setState(STOPPED);
}

void Arm::idle() {
	//do nothing
}

bool Arm::isIdle() {
	return (state == IDLE_OPEN || state == IDLE_CLOSED);
}

void Arm::setState(State newState) {
	if(state!=STOPPED) {
		state = newState;
	}
}

void Arm::toggle() {
	switch(state) {
		case IDLE_OPEN:
			close();
			break;
		case OPENING:
			close();
			break;
		case IDLE_CLOSED:
			open();
			break;
		case CLOSING:
			open();
			break;
	}
}

void Arm::open() {
	setState(OPENING);
	timer.Start();
}

void Arm::close() {
	setState(CLOSING);
	timer.Start();
}

Arm::~Arm() {

}

