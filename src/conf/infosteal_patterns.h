#pragma once
#include "../types/types.h"

namespace conf {

	const std::vector<ifs_pattern_entry> infosteal_patterns = {

		ifs_pattern_entry("generic_credential",std::regex("^(?:POST|GET)[\\s\\S]*(?:(\\r)?\\n){2}[\\s\\S]*(?:username|user|uname|name|)?=[\\s\\S]*(?:password|pass|pwd|secret)=[\\s\\S]*$",std::regex_constants::icase))

	};
}