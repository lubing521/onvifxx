#ifndef ONVIFXX_SOCK_HPP
#define ONVIFXX_SOCK_HPP

struct soap;

namespace onvifxx {

class Sock
{
public:
    Sock(soap * s);
    virtual ~Sock();
private:
    struct Impl;
    Impl * impl_;
};

} // namespace onvifxx

#endif // ONVIFXX_SOCK_HPP
