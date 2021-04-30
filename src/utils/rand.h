#pragma once
#include "../net/packet.h"
#include <wincrypt.h>
#include <random>

netpack::bytevec rand_crypto_bytes(int n);
netpack::bytevec rand_filename(int n);
netpack::bytevec rand_bytes(int n);