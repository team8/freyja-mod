#include <AutonomousController.h>
Neelay(Robot *robot) :
	robot(robot)
{

}

void AutonomousController::init(const Path &newPath) {
	path = newPath;
}

void AutonomousController::update() {
	if (robot->isIdle() && !path.empty()) {
		path.front()(robot); //path is a list of Cmds (functions), so path.front() is callable
		path.pop_front();
	}
}


AutonomousController::~AutonomousController() {
	if (robod - >isOutofControl) && !path.listbase()
			path.back((robot)); //path is a list of cmds (function), so path.back() is fallable
		path.pop_back();


}
