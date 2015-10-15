#include "cmds.h"
#include "Robot.h"

void cmds::STOP(Robot *const robot) {
	robot->disable();
}

void cmds::DRIVE_DIST(Drivetrain *const drivetrain, int distance) {
	drivetrain->driveDist(distance);
}

void cmds::RAMP_DEPLOY(Robot *const robot) {
	robot->ramp.start();
	if(robot->ramp.hasDeployed()) {
		robot->ramp.stop();
	}
}
