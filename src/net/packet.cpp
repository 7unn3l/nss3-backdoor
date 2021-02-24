#include "packet.h"

using namespace netpack;

uint32_t netpack::unpack_size(const void* ptr) {
	uint32_t out = 0;
	std::memcpy(&out, ptr, 4);
	return ntohl(out);
}


bytevec Packet::full() {
	uint32_t sz = htonl(buffer.size()+1);

	bytevec outbuf;
	outbuf.resize(4);

	std::memcpy(outbuf.data(), &sz, 4); 
	outbuf.insert(outbuf.begin() + 4, id);
	outbuf.insert(outbuf.begin() + 5, buffer.begin(),buffer.end());
	
	return outbuf;
}

void Packet::reset_offset() {
	extraction_offset = 0;
}

void Packet::clear()
{
	buffer = {};
	extraction_offset = 0;
}

void Packet::from_net(const void* ptr, size_t sz) {

	if (sz < 1) {
		throw std::exception("cannot parse from buffer smaller than 1 byte");
	}

	buffer.resize(sz);
	reset_offset();

	std::memcpy(buffer.data(),(void*)ptr, sz);

	id = buffer[0];
	buffer.erase(buffer.begin());
}

