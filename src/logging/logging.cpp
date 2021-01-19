#include "logging.h"

namespace logger {
    using asio::ip::udp;

    asio::io_context io_context;
    udp::endpoint loggingserver;
    udp::socket logsock(io_context);

    void debug_msg(std::string msg) {

        if (msg.size() > 65535) {
            //implement fragmentation
            logsock.send(asio::buffer("[!] Held back message that was too large to send"));
            return;
        }
        logsock.send(asio::buffer(msg));
    }

    void initsocket() {
        loggingserver = udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 6969);
        logsock.connect(loggingserver);
    }
}