#include "udp.hpp"
#include <onvifxx/onvifxx.hpp>

namespace onvifxx {

Udp::Udp() :
    socket_(ios_)
{
}

bool Udp::isUdp() const
{
    return true;
}

Ip::Result_t Udp::connect(const std::string & host, unsigned short port)
{
    using namespace boost::asio::ip;

    Result_t rv;
    const auto addr = address::from_string(host, rv);
    if (rv)
        return rv;

    endpoint_ = udp::endpoint(addr, port);
    socket_.connect(endpoint_, rv);

    return rv;
}

Ip::Result_t Udp::close()
{
    Result_t rv;
    socket_.close(rv);

    return rv;
}

Ip::Result_t Udp::send(const char * data, std::size_t size)
{
    Ip::Result_t rv;
    socket_.send_to(boost::asio::buffer(data, size), endpoint_, 0, rv);

    return rv;
}


Ip::Result_t Udp::recv(char * data, std::size_t size)
{
    Ip::Result_t rv;
    socket_.receive_from(boost::asio::buffer(data, size), endpoint_, 0, rv);

    return rv;
}


} // namespace onvifxx

