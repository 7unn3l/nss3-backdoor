#pragma once
#include <string>

struct Match {
	std::string host;
	std::vector<std::string> capturegroups;
};