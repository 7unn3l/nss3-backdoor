#include "extract.h"
struct Match;

std::string read_from_cbuf(void* buf, int amnt) {
	std::string a = "";

	for (int i = 0; i < amnt; i++) {
		a += ((unsigned char*)buf)[i];
	}
	return a;
}

std::vector<Match> extract_info(void* b,std::vector<ifs_pattern_entry> patterns,int amnt) {

	std::string buf = read_from_cbuf(b, amnt);
	std::vector<Match> outvec;
	
	
	for (int i = 0; i < patterns.size(); i++) {

		std::smatch matches;
		std::smatch hmatches;

		ifs_pattern_entry element = patterns[i];
		
		if (std::regex_search(buf, matches, element.second)) {
			LOG("----------------------------");
			LOG("found match for " << element.first);

			Match m;

			m.name = element.first;

			//try to get host
			if (std::regex_search(buf, hmatches, std::regex("Host: (.*)"))) {
				LOG("could extract host: " << hmatches[1]);
				m.host = hmatches[1];
			}

			for (size_t i = 0; i < matches.size(); i++) {
				std::string creds = matches[i];
				LOG("group " << i << " : " << creds);
				m.capturegroups.push_back(creds);
			}

			outvec.push_back(m);
		}

	}
	return outvec;
};
