#ifndef SUBSYS_H
#define SUBSYS_H

#include <iostream>

/**
 * Base class for all Subsystems
 * Declares certain functions common to all Subsystems
 */
class Subsys {
public:

	Subsys();
	virtual ~Subsys();

	// virtual = 0 means the function is overridable and must be implemented in non-abstract base classes

	/**
	 * Initialization of subsystem
	 */
	virtual void init() = 0; 

	/**
	 * To update the subsytem in periodic
	 */
	virtual void update() = 0;

	/**
	 * To disable the system during disabled
	 */
	virtual void disable() = 0;

	virtual void idle() = 0;

	/**
	 * @return whether the subsystem is in state IDLE
	 */
	virtual bool isIdle() = 0;



};

#endif
