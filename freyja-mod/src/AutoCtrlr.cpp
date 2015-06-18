#include "AutoCtrlr.h"

AutoCtrlr::AutoCtrlr() {

}

AutoCtrlr::~AutoCtrlr() {

}

void AutoCtrlr::init(const Path &new_path) {
	path = new_path;
}

void AutoCtrlr::update(Robot *const robot) {
	if (robot->is_idle() && !path.empty()) {
		path.front()(robot); //path is a list of Cmds (functions), so path.front() is callable
		path.pop_front();
	}
}
