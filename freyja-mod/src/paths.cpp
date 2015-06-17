#include "paths.h"

template <typename F, typename ... R> Path paths::build_path(const F &first, const R& ... rest) {
	return combine_paths(to_path(first), build_path(rest...));
}

template <typename T> Path paths::build_path(const T &t) {
	return to_path(t);
}

Path paths::build_path() {
	return Path{};
}

Path paths::combine_paths(Path lhs, const Path &rhs) {
	for (const Cmd &cmd : rhs)
		lhs.push_back(cmd);
	return lhs;
}

Path paths::to_path(const Cmd &cmd) {
	return Path{cmd};
}

Path paths::to_path(const Path &path) {
	return path;
}
