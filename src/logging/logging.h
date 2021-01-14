#pragma once
#include <string>
#include "asio.hpp"

#define LOG(msg) logger::debug_msg((std::ostringstream() << msg).str())
using asio::ip::udp;

namespace logger {

    asio::io_context io_context;
    udp::endpoint loggingserver;
    udp::socket logsock(io_context);

    void debug_msg(std::string msg) {
        logsock.send(asio::buffer(msg));
    }

    void initsocket() {
        loggingserver = udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 6969);
        logsock.connect(loggingserver);
    }
}