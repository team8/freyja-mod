#ifndef TELE_CONTROLLER_H
#define TELE_CONTROLLER_H

#include "WPILib.h"
#include "Robot.h"
#include "paths.h"

class TeleController {
public:

	TeleController();
	~TeleController();

	/**
	 * Currently doesn't actually do anything
	 */
	void init();

	/**
	 * Commands the robot's actions based on human input
	 * @param robot The pointer to the robot which will be controlled by the TeleController
	 */
	void update(Robot *const robot);

};

#endif
