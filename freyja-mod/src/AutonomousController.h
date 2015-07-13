#ifndef AUTONOMOUS_CONTROLLER_H
#define AUTONOMOUS_CONTROLLER_H

#include "Robot.h"
#include "paths.h"

class AutonomousController {
private:

	/**
	 * The path to be run in autonomous
	 */
	Path path;

	Robot *robot;

public:

	AutonomousController(Robot *robot);
	~AutonomousController();

	/**
	 * Sets the Path for the autonomous period
	 * @param new_path The path for the autonomous period
	 */
	void init(const Path &newPath);

	/**
	 * Executes the next Cmd in path, if there is one
	 * @param robot The pointer to the robot which will be controlled by the AutonomousController
	 */
	void update();
};

#endif //AUTO_CONTROLLER_H
