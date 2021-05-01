#include "cachemanager.h"

CacheManager::CacheManager(int mf) : max_files{mf}
{
	//set fsroot
	auto tmpdir = fs::temp_directory_path();
	int spec_val = ((conf::dgaseed * 2) % 512)+1;
	auto subdir = fs::path(dga(spec_val));

	fsroot = (tmpdir / subdir).string();
	make_fsroot();
}

int CacheManager::cache(netpack::Packet& tocache)
{
	std::string fname = rand_avail_filename();

	auto netdata = tocache.full();
	size_t sz = netdata.size();

	LOG("caching packet of size " << sz << " into " << fname);

	std::fstream outfile;
	outfile.open(fname,std::ios::app |std::ios::binary);
	outfile.write((char*)netdata.data(), sz);
	outfile.close();

	return 0;
}

std::string CacheManager::rand_avail_filename() {
	stringvec files = list_files(fsroot);
	for (;;) {
		std::string fname = rand_filename(10);
		if (std::find(files.begin(), files.end(), fname) == files.end()) {
			return (fs::path(fsroot) / fs::path(fname)).string();
		}
	}
};

void CacheManager::make_fsroot()
{
	if (!fs::exists(fsroot)){
		fs::create_directories(fsroot);
	}
}

stringvec CacheManager::list_files(const std::string& dir)
{
	stringvec out;
	for (const auto& entry : fs::directory_iterator(dir)) {

		if (entry.is_regular_file()) {
			out.push_back(entry.path().filename().string());
		}

	}
	return out;
}

stringvec CacheManager::list_folders(const std::string& dir) {
	stringvec out;
	for (const auto& entry : fs::directory_iterator(dir)) {

		if (entry.is_directory()) {
			out.push_back(entry.path().filename().string());
		}
	}
	return out;
}
