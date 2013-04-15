#include "tcp.hpp"
#include <onvifxx/onvifxx.hpp>

namespace onvifxx {

Tcp::Tcp() :
    socket_(ios_)
{
}

Ip::Result_t Tcp::connect(const std::string & host, unsigned short port)
{
    using namespace boost::asio::ip;

    Result_t rv;
    const auto addr = address::from_string(host, rv);
    if (rv)
        return rv;

    tcp::endpoint ep(addr, port);
    socket_.connect(ep, rv);

    return rv;
}

Ip::Result_t Tcp::close()
{
    Result_t rv;
    socket_.close(rv);

    return rv;
}

Ip::Result_t Tcp::send(const char * data, std::size_t size)
{
    Ip::Result_t rv;
    socket_.send(boost::asio::buffer(data, size), 0, rv);

    return rv;
}

Ip::Result_t Tcp::recv(char * data, std::size_t size)
{
    Ip::Result_t rv;
    socket_.receive(boost::asio::buffer(data, size), 0, rv);

    return rv;
}

} // namespace onvifxx
