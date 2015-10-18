#include "cmds.h"
#include "Robot.h"
#include "Subsys.h"

void cmds::STOP(Robot *const robot) {
	robot->disable();
	robot->endTrout();
}

void cmds::DRIVE_DIST(Drivetrain *const drivetrain, int distance) {
	drivetrain->driveDist(distance);
}

bool wait(Subsys *const task) {
	std::vector<Subsys> *endable = {task};
	return wait(task, endable);
}

bool wait(Subsys *const task, std::vector<Subsys> *endable) {
	while(!task->isIdle()) {
		for(int i = 0; i < endable->size(); i++) {
			if(((Subsys)endable[i]).endTrout()) {
				return false;
			}
		}
	}
	return true;
}
