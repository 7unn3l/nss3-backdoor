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
	std::smatch matches;
	
	for (int i = 0; i < patterns.size(); i++) {
		ifs_pattern_entry element = patterns[i];
		
		if (std::regex_search(buf, matches, element.second)) {
			LOG("found match for " << element.first);

			for (size_t i = 0; i < matches.size(); i++) {
				std::string creds = matches[i];
				LOG("group " << i << " : " << creds);
			}

		}

	}
	/*std::vector<Match> vec;
	Match m{};
	m.host = "123";
	vec.push_back(m);
	return vec;*/
	return std::vector<Match>{};
};
