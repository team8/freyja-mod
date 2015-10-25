#include "paths.h"

template <typename F, typename ... R> Path paths::buildPath(const F &first, const R& ... rest) {
	return combinePaths(toPath(first), buildPath(rest...));
}

template <typename T> Path paths::buildPath(const T &t) {
	return toPath(t);
}

Path paths::combinePaths(Path lhs, const Path &rhs) {
	for (const Cmd &cmd : rhs)
		lhs.push_back(cmd);
	return lhs;
}

Path paths::toPath(const Cmd &cmd) {
	return Path{cmd};
}

Path paths::toPath(const Path &path) {
	return path;
}
