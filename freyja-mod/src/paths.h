#ifndef AUTO_PATHS
#define AUTO_PATHS

#include "cmds.h"

//Predefined Paths
namespace paths { 

	/**
	 * Combines 2 paths into 1, by appending the 2nd one onto the 1st
	 */
	Path combinePaths(Path lhs, const Path &rhs);

	/**
	 * Converts the Cmd to a Path consisting of only that Cmd, or
	 * does nothing if the param is a Path
	 * @return the resulting path
	 */
	Path toPath(const Cmd &cmd);
	Path toPath(const Path &path);

	/**
	 * buildPath is used to make Paths that are comprised of other Paths (then subpaths) and/or Cmds
	 * Infinite arguments, any combination
	 */
	template <typename T> Path buildPath(const T &t);
	template <typename F, typename ... R> Path buildPath(const F &first, const R& ... rest);

	/**
	 * Paths
	 */

	const Path NOTHING = {cmds::STOP};
	const Path DRIVE = {cmds::DRIVE_DIST};
	const Path LIFT = {cmds::LIFT_UP};

	const Path TURN_LEFT = {cmds::TURN_LEFT};
	const Path TURN_RIGHT = {cmds::TURN_RIGHT};

	//Position for can pickup
	const Path POSITION = {cmds::DRIVE_SHORT_DIST};
	const Path PICKUP_CAN = combinePaths(LIFT, DRIVE);
	//Lifts can in front of robot then drives forward
	const Path AUTO = combinePaths(POSITION, PICKUP_CAN);

	//Drive into auto zone from landfill
	const Path DRIVE_LANDFILL= {cmds::DRIVE_LANDFILL};
	//Move into auto zone then turn
	const Path ROTATE_AUTO_ZONE = combinePaths(DRIVE_LANDFILL, TURN_RIGHT);
	//Cross the autozone
	const Path DRIVE_AUTO_ZONE = {cmds::DRIVE_AUTO_ZONE};
	//Starting in landfill, enter auto zone, end away from step
	const Path LANDFILL_AUTO = buildPath(LIFT, ROTATE_AUTO_ZONE, DRIVE_AUTO_ZONE, cmds::ALIGN_CHUTE);
}


#endif //AUTO_PATHS
