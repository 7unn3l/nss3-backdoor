#pragma once
#include "../types/types.h"
#include "../logging/logging.h"
#include "match.h"

struct Match;

std::vector<Match> extract_info(void* b, std::vector<ifs_pattern_entry> patterns,int);