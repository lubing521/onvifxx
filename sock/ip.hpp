#ifndef ONVIFXX_IP_HPP
#define ONVIFXX_IP_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace onvifxx {

class Ip
{
public:
    typedef boost::system::error_code Result_t;

    virtual ~Ip();

    virtual bool isUdp() const;

    virtual Result_t connect(const std::string & host, unsigned short port) = 0;
    virtual Result_t close() = 0;

    virtual Result_t send(const char * data, std::size_t size) = 0;
    virtual Result_t recv(char * data, std::size_t size) = 0;

protected:
    boost::asio::io_service ios_;
};

} // namespace onvifxx

#endif // ONVIFXX_IP_HPP
