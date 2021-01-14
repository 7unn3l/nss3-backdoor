#include "intercepted.h"
#include "proxied.h"
#include "types/types.h"
#include "logging/logging.h"

pr_io_func pr_write_orig = 0;
pr_io_func pr_read_orig = 0;

int myPR_WRITE(void* fd, void* buf, int amnt) {
	LOG("write was called");
	int f = pr_write_orig(fd, buf, amnt);
	return f;
};

int myPR_READ(void* fd, void* buf, int amnt) {
	int f = pr_read_orig(fd, buf, amnt);
	return f;
};


BOOL WINAPI DllMain(HINSTANCE mod, DWORD fdwReason, LPVOID resv) { 

	if (fdwReason == DLL_PROCESS_ATTACH){

		logger::initsocket();

		LOG("dllmain called with PROCESS_ATTACH");

		//load functions from original dll (eventually threaded)
		HINSTANCE hGetProcIDDLL = LoadLibraryA("nss3.orig");
		pr_read_orig = (pr_io_func)GetProcAddress(hGetProcIDDLL, "PR_Read");
		pr_write_orig = (pr_io_func)GetProcAddress(hGetProcIDDLL, "PR_Write");
	}
	
	return TRUE;
}