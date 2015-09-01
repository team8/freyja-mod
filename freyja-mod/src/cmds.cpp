#include "cmds.h"
#include "Robot.h"

void cmds::STOP(Robot *const robot) {
	robot->disable();
}


template<int N> void cmds::DRIVE_DIST(Robot *const robot) {
	robot->drivetrain.driveDist(N);
}
