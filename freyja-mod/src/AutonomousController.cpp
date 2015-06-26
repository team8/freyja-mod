#include <AutonomousController.h>

AutonomousController::AutonomousController() {

}

void AutonomousController::init(const Path &newPath) {
	path = newPath;
}

void AutonomousController::update(Robot *const robot) {
	if (robot->isIdle() && !path.empty()) {
		path.front()(robot); //path is a list of Cmds (functions), so path.front() is callable
		path.pop_front();
	}
}


AutonomousController::~AutonomousController() {

}
