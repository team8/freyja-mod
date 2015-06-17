#include "Robot.h"

Robot::Robot():
	state{NOTHING},
	path(paths::NOTHING)
{

}

Robot::~Robot() {

}

void Robot::init() {
	state = TELE;
}

void Robot::update() {
	switch (state) {
		case NOTHING:
			break;
		case AUTO: //fallthrough correct
		case TELE:
			//update Subsystems here
			break;
		case TROUT: //Teleoperated Routines
			if (is_idle() && !path.empty()) {
				path.front()(this);
				path.pop_front();
			}

			//update Subsystems here

			if (path.empty())
				state = TELE;
			break;
		default:
			std::cerr << "error: Robot in nonexistant state" << std::endl;
			break;
	}
}

void Robot::disable() {
	//disable Subsystems here
}

bool Robot::is_idle() const {
	//return whether all Subsystems are idle here
	return true;
}

void Robot::set_state(const State &new_state) {
	if (new_state == TROUT) {
		disable();
		state = new_state;
	} else
		state = new_state;
}

void Robot::set_path(const Path &new_path) {
	path = new_path;
}
