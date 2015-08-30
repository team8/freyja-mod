#include "Robot.h"

Robot::Robot():
	state(NOTHING),
	path(paths::NOTHING),
	drivetrain(),
	lifter(),
	accel()

{

}

Robot::~Robot() {

}

void Robot::init() {
	state = TELEOP;
}

void Robot::update() {
	switch (state) {
		case NOTHING:
			break;
		case AUTONOMOUS: //fallthrough correct
		case TELEOP:
			//update Subsystems here
			break;
		case TROUT: //Teleoperated Routines
			if (isIdle() && !path.empty()) {
				path.front()(this);
				path.pop_front();
			}

			//update Subsystems here

			if (path.empty())
				state = TELEOP;
			break;
		default:
			std::cerr << "error: Robot in nonexistant state" << std::endl;
			break;
	}

	//Stops the robot from flipping
	if(accel.GetZ() > ACCELEROMETER_FLIP_LIMIT && lifter.getVelocity() < LIFTER_FLIP_LIMIT) {
		lifter.setVelocity(0);
	}

}

void Robot::disable() {
	lifter.disable();
	drivetrain.disable();
}

bool Robot::isIdle() const {
	//return whether all Subsystems are idle here
	return true;
}

void Robot::setState(const State &newState) {
	if (newState == TROUT) {
		disable();
		state = newState;
	} else
		state = newState;
}

void Robot::setPath(const Path &newPath) {
	path = newPath;
}
