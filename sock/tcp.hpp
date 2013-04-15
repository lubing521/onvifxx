#ifndef ONVIFXX_TCP_HPP
#define ONVIFXX_TCP_HPP

#include "ip.hpp"

namespace onvifxx {

class Tcp :
        public Ip
{
public:
    Tcp();

    virtual Result_t connect(const std::string & host, unsigned short port);
    virtual Result_t close();

    virtual Result_t send(const char * data, std::size_t size);
    virtual Result_t recv(char * data, std::size_t size);

private:
    boost::asio::ip::tcp::socket socket_;
};

} // namespace onvifxx

#endif // ONVIFXX_TCP_HPP
