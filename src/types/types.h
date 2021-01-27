#pragma once
#include <regex>

typedef int(__stdcall* pr_io_func)(void*, void*, int);
typedef std::pair<const char*, std::regex> ifs_pattern_entry;
typedef std::vector<std::string> stringvec;