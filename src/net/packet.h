#pragma once
#include <winsock.h>
#include <string>
#include <vector>

#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))


namespace netpack {

	typedef std::vector<uint8_t> bytevec;

	uint32_t unpack_size(const void* ptr);

	class Packet {

	public:
		bytevec buffer;
		uint8_t id;
		Packet(uint8_t i) : id{ i } {};

		template <typename Datatype>
		friend Packet& operator << (Packet& packet, Datatype data) {
			static_assert(std::is_standard_layout<Datatype>::value, "cannot serialze datatype");

			switch (sizeof(data))
			{
			case 1:
				break;
			case 2:
				data = ntohs(data);
				break;
			case 4:
				data = ntohl(data);
				break;
			case 8:
				data = ntohll(data);
				break;
			}


			size_t typesize = sizeof(data);

			return packet.insert_val(&data, typesize);
		}

		friend Packet& operator << (Packet& packet, const char* data) {
			uint32_t stlen = strlen(data);

			packet << stlen;

			return packet.insert_val(data, stlen);
		}

		friend Packet& operator << (Packet& packet, const std::string data) {
			uint32_t stlen = data.size();

			packet << stlen;

			return packet.insert_val(data.data(), stlen);
		}


		template <typename Datatype>
		friend Packet& operator >> (Packet& packet, Datatype& data) {

			static_assert(std::is_arithmetic<Datatype>::value, "data can only be serialized into numeric types and std::string");

			bytevec rawdata{};
			rawdata = packet._next_n_bytes(sizeof(data));

			Datatype tmpdataholder;

			std::memcpy(&tmpdataholder, rawdata.data(), sizeof(data));

			switch (sizeof(data))
			{
			case 1:
				break;
			case 2:
				tmpdataholder = ntohs(tmpdataholder);
				break;
			case 4:
				tmpdataholder = ntohl(tmpdataholder);
				break;
			case 8:
				tmpdataholder = ntohll(tmpdataholder);
				break;
			}

			std::memcpy(&data, &tmpdataholder, sizeof(data));

			return packet;
		}

		friend Packet& operator >> (Packet& packet, std::string& data) {
			uint32_t sz = 0;
			packet >> sz;
			bytevec buf = packet._next_n_bytes(sz);
			data.assign(buf.data(), buf.data() + sz);

			return packet;
		}

		bytevec full();
		void reset_offset();
		void clear();
		void from_net(const void* ptr,size_t sz);

	private:
		
		size_t extraction_offset = 0;

		template <typename Datatype>
		Packet& insert_val(Datatype* dataptr, size_t datasize) {
			size_t bufsize = this->buffer.size();

			this->buffer.resize(bufsize + datasize);

			std::memcpy(this->buffer.data() + bufsize, dataptr, datasize);

			return *this;
		}

		bytevec _next_n_bytes(size_t n) {
			bytevec out;
			size_t bytes_available = buffer.size() - extraction_offset;

			if (bytes_available < n) {
				throw std::exception("not enough bytes available to perfrom extraction");
				return out;
			}

			for (int i = 0; i < n; i++) {
				out.push_back(buffer[extraction_offset + i]);
			}

			extraction_offset += n;

			return out;
		}

	};

}