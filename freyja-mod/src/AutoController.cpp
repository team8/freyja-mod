#include <AutoController.h>

AutoController::AutoController() {

}

AutoController::~AutoController() {

}

void AutoController::init(const Path &newPath) {
	path = newPath;
}

void AutoController::update(Robot *const robot) {
	if (robot->isIdle() && !path.empty()) {
		path.front()(robot); //path is a list of Cmds (functions), so path.front() is callable
		path.pop_front();
	}
}
