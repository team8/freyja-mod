#ifndef TELE_CTRLR_H
#define TELE_CTRLR_H

#include "WPILib.h"
#include "Robot.h"
#include "paths.h"

class TeleCtrlr {
public:

	TeleCtrlr();

	~TeleCtrlr();

	/**
	 * Currently doesn't actually do anything
	 */
	void init();

	/**
	 * Commands the robot's actions based on human input
	 * @param robot The pointer to the robot which will be controlled by the TeleCtrlr
	 */
	void update(Robot *const robot);

};

#endif
