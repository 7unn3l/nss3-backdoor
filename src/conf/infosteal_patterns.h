#pragma once
#include "../types/types.h"

namespace conf {

	const std::vector<ifs_pattern_entry> infosteal_patterns = {

		ifs_pattern_entry("generic_password",std::regex("^.*(?:password|pwd|passwd|pass)=.*$"))

	};
}