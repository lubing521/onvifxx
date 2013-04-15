#ifndef ONVIFXX_UDP_HPP
#define ONVIFXX_UDP_HPP

#include "ip.hpp"

namespace onvifxx {

class Udp :
        public Ip
{
public:
    Udp();

    virtual bool isUdp() const;

    virtual Result_t connect(const std::string & host, unsigned short port);
    virtual Result_t close();

    virtual Result_t send(const char * data, std::size_t size);
    virtual Result_t recv(char * data, std::size_t size);

private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint endpoint_;
};

} // namespace onvifxx

#endif // ONVIFXX_UDP_HPP
