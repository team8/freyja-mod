#ifndef AUTO_CONTROLLER_H
#define AUTO_CONTROLLER_H

#include "Robot.h"
#include "paths.h"

class AutoController {
private:

	/**
	 * The path to be run in autonomous
	 */
	Path path;

public:

	AutoController();

	~AutoController();

	/**
	 * Sets the Path for the autonomous period
	 * @param new_path The path for the autonomous period
	 */
	void init(const Path &newPath);

	/**
	 * Executes the next Cmd in path, if there is one
	 * @param robot The pointer to the robot which will be controlled by the AutoController
	 */
	void update(Robot *const robot);
};

#endif //AUTO_CONTROLLER_H
