#include <AutonomousController.h>

AutonomousController::AutonomousController(Robot *robot) :
	robot(robot)
{

}

void AutonomousController::init(const Path &newPath) {
	path = newPath;
	robot->init();
}

void AutonomousController::update() {
	robot->drivetrain.update();
	robot->lifter.update();
	if (robot->isIdle() && !path.empty()) {
		path.front()(robot); //path is a list of Cmds (functions), so path.front() is callable
		path.pop_front();
	}
}


AutonomousController::~AutonomousController() {

}
