#include "cmds.h"
#include "Robot.h"

void cmds::STOP(Robot *const robot) {
	robot->disable();
	robot->endTrout();
}

void cmds::DRIVE_DIST(Drivetrain *const drivetrain, int distance) {
	drivetrain->driveDist(distance);
}
