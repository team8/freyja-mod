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

}


#endif //AUTO_PATHS
