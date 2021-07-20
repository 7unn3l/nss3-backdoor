#pragma once
#include <string>

struct Match {
	std::string name;
	std::string host;
	uint64_t timestamp;
	std::vector<std::string> capturegroups;
};