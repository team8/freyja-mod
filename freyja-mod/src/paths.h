#ifndef AUTO_PATHS
#define AUTO_PATHS

#include "cmds.h"

//Predefined Paths
namespace paths { 

	/**
	 * Combines 2 paths into 1, by appending the 2nd one onto the 1st
	 */
	Path combine_paths(Path lhs, const Path &rhs);

	/**
	 * Converts the Cmd to a Path consisting of only that Cmd, or
	 * does nothing if the param is a Path
	 * @return the resulting path
	 */
	Path to_path(const Cmd &cmd);
	Path to_path(const Path &path);

	/**
	 * build_path is used to make Paths that are comprised of other Paths (then subpaths) and/or Cmds
	 * Infinite arguments, any combination
	 */
	template <typename T> Path build_path(const T &t);
	template <typename F, typename ... R> Path build_path(const F &first, const R& ... rest); 

	/**
	 * Paths
	 */

	const Path NOTHING = {cmds::STOP};

}


#endif //AUTO_PATHS
