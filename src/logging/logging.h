#pragma once
#include <string>
#include "asio.hpp"

#ifdef DEBUG

#define LOG(msg) logger::debug_msg((std::ostringstream() << msg).str())
#define INITLOG() logger::initsocket()

namespace logger {

    void debug_msg(std::string msg);
    void initsocket();
}
#else
#define LOG(msg);
#define INITLOG();
#endif