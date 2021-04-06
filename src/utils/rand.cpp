#include "rand.h"

netpack::bytevec rand_bytes(int n)
{
	netpack::bytevec out(n,0);

	//use default csp
	HCRYPTPROV cryptprov = 0;

	if (CryptAcquireContext(&cryptprov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		CryptGenRandom(cryptprov, n, out.data());
		CryptReleaseContext(cryptprov, 0);
	}
	
	return out;
}
