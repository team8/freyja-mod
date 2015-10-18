#ifndef CMDS_H
#define CMDS_H

#include <functional>
#include <list>
#include <vector>

//forward declaring Robot and subsystems
class Robot;
class Subsys;
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
	void DRIVE_DIST(Drivetrain *const drivetrain, int distance);

	/**
	 * Wait for a subsystem (task) to become idle before performing next task
	 * Also interrupts trout if any subsytem in "endable" activates manually
	 * Called by trouts
	 */
	bool wait(Subsys *const task);
	bool wait(Subsys *const task, std::vector<Subsys> *endable);
}


#endif //CMDS_H
