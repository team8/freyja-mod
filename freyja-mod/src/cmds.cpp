#include "cmds.h"
#include "Robot.h"

void cmds::STOP(Robot *const robot) {
	robot->disable();
	robot->popTrout();
}

void cmds::DRIVE_DIST(Drivetrain *const drivetrain, int distance) {
	drivetrain->driveDist(distance);
}

void cmds::LIFT_ONE(Robot *const robot) {
	robot->lifter.zero();
	while(!robot->lifter.isIdle()){}
	robot->lifter.levelUp();
	while(!robot->lifter.isIdle()){}
	robot->lifter.levelDown();
	robot->popTrout();
}
