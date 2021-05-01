#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include "../conf/c2.h"
#include "../logging/logging.h"
#include "../utils/rand.h"
#include "../types/types.h"
#include "../c2/dga.h"
#include "../net/packet.h"


namespace fs = std::filesystem;

class CacheManager {
public:
	CacheManager(int max_files);

	int cache(netpack::Packet& tocache);
	int remove(const std::string id);
	std::vector<netpack::bytevec> get_all_cached();

private:
	std::string fsroot ;
	std::string private_folder;
	const int max_files;

	void make_fsroot();

	stringvec list_files(const std::string& dir);
	stringvec list_folders(const std::string& dir);
	std::string rand_avail_filename();

};