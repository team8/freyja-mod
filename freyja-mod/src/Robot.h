#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "paths.h"
#include "Drivetrain.h"
#include "Lifter.h"
#include "Ramp.h"

class Robot {
public:

	Robot();

	~Robot();

	/**
	 * The possible states the Robot can be in
	 */
	enum State {
		NOTHING,
		AUTONOMOUS,
		TELEOP,
		TROUT //Tele Routines
	};

	/**
	 * The state the robot is in
	 */
	State state;

	void setState(const State &newState);

	/**
	 * Initializes the robot, and sets it to AUTO/TELE
	 */
	void init();

	/**
	 * If in state AUTO or TELE, updates subsystems
	 * If in state TROUT, executes the next Cmd in path, if there is one
	 */
	void update();

	/**
	 * Disables all subsystems
	 */
	void disable();

	/**
	 * @return Returns whether all subsystems are idle
	 */
	bool isIdle();

	/**
	 * If a Teleop Routine is given by TeleCtrlr, path stores the Path given
	 */
	Path path;

	void setPath(const Path &newPath);

	// The four subsystems
	Drivetrain drivetrain;
	Lifter lifter;
	Ramp ramp;
};

#endif
