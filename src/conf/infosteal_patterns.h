#pragma once
#include "../types/types.h"

namespace conf {

	const std::vector<ifs_pattern_entry> infosteal_patterns = {

		ifs_pattern_entry("generic_passwordfield",std::regex("^.*(?:password|pwd|passwd|pass)=.*$",std::regex_constants::icase)),
		ifs_pattern_entry("generic_login_post",std::regex("POST .*(?:login|auth).*[\\s\\S]*",std::regex_constants::icase))

	};
}