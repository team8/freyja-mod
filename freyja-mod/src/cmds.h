#ifndef CMDS_H
#define CMDS_H

#include <functional>
#include <list>

//forward declaring Robot and subsystems
class Robot;
class Drivetrain;
class Lifter;
class Ramp;
class Arm;

//aliases the function to Cmd
using Cmd = std::function<void(Robot *const)>;

//aliases the list to Path
using Path = std::list<Cmd>;

//Predefined Commands
namespace cmds {

	/**
	 * A pointer to the robot is passed to each command.
	 * This pointer is immutable, while the robot it points to is mutable.
	 */

	/**
	 * Disables the robot
	 */
	void STOP(Robot *const robot);

	/**
	 * Drives the robot a set distance
	 */
	void DRIVE_DIST(Robot *const robot);

	void DRIVE_SHORT_DIST(Robot *const robot);

	/**
	 * Deploys a tote from the ramp
	 */
	void RAMP_DEPLOY(Robot *const robot);

	void LIFT_UP(Robot *const robot);

	/**
	 * Turns left/right 90 deg
	 */
	void TURN_LEFT(Robot *const robot);
	void TURN_RIGHT(Robot *const robot);
}


#endif //CMDS_H
