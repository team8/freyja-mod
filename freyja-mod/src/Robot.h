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
	bool is_idle() const;

	void set_state(const State &new_state);

	/**
	 * If a Teleop Routine is given by TeleCtrlr, path stores the Path given
	 */
	Path path;

	void set_path(const Path &new_path); 

};

#endif
