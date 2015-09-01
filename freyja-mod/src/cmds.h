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
	 * Example 1:
	 * 	Path path {DRIVE_DIST<10>}
	 *
	 * Example 2:
	 * 	Cmd DRIVE10 = DRIVE_DIST<10>; //name the cmd something related to the function of driving that specific distance
	 * 	Path path { DRIVE10 };
	 */
	template<int N> void DRIVE_DIST(Robot *const robot);
}


#endif //CMDS_H
