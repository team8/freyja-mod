#include "cmds.h"
#include "Robot.h"

void cmds::STOP(Robot *const robot) {
	robot->disable();
}
