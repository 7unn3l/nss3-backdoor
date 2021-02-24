#pragma once
#include <string>

struct Match {
	std::string name;
	std::string host;
	std::vector<std::string> capturegroups;
};