#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#ifdef nss3_EXPORTS
#define nss3_API __declspec(dllexport)
#else
#define nss3_API __declspec(dllimport)
#endif

extern "C" {
	nss3_API int myPR_WRITE(void* fd, void* buf, int amnt);
	nss3_API int myPR_READ(void* fd, void* buf, int amnt);
}