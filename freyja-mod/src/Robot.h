#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "paths.h"

class Robot {
public:

	Robot();

	~Robot();

	/**
	 * The possible states the Robot can be in
	 */
	enum State {
		NOTHING,
		AUTO,
		TELE,
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
	bool isIdle() const;

	/**
	 * If a Teleop Routine is given by TeleCtrlr, path stores the Path given
	 */
	Path path;

	void setPath(const Path &newPath);

};

#endif
