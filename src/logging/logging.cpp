#include "logging.h"

namespace logger {
    using asio::ip::udp;

    asio::io_context io_context;
    udp::endpoint loggingserver;
    udp::socket logsock(io_context);

    void debug_msg(std::string msg) {
        msg += "\n";

        size_t offset = 0;
        size_t bs = 1024;

        while (offset + bs < msg.size()) {

            std::string s = msg.substr(offset, bs);
            logsock.send(asio::buffer(s));

            offset += bs;

        }

    }


    void initsocket() {
        loggingserver = udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 6969);
        logsock.connect(loggingserver);
    }
}