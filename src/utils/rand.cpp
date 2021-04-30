#include "rand.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> char_dist(0, 63);
std::uniform_int_distribution<> byte_dist(0, 63);

static const char fname_charpool[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"_";

netpack::bytevec rand_filename(int n) {
	netpack::bytevec fname;
	for (; n != 0; n--) {
		fname.push_back(fname_charpool[char_dist(gen)]);
	}
	return fname;
}

netpack::bytevec rand_crypto_bytes(int n)
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

netpack::bytevec rand_bytes(int n) {
	netpack::bytevec data;
	for (; n != 0; n--) {
		data.push_back(byte_dist(gen));
	}
	return data;
}