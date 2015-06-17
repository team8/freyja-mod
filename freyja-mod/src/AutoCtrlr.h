#ifndef AUTO_CTRLR_H
#define AUTO_CTRLR_H

#include "Robot.h"
#include "paths.h"

class AutoCtrlr {
private:

	/**
	 * The path to be run in autonomous
	 */
	Path path;

public:

	AutoCtrlr();

	~AutoCtrlr();

	/**
	 * Sets the Path for the autonomous period
	 * @param new_path The path for the autonomous period
	 */
	void init(const Path &new_path);

	/**
	 * Executes the next Cmd in path, if there is one
	 * @param robot The pointer to the robot which will be controlled by the AutoCtrlr
	 */
	void update(Robot *const robot); //
};


#endif //AUTO_CTRLR_H
