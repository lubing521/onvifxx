#include "ip.hpp"
#include <onvifxx/onvifxx.hpp>

namespace onvifxx {

Ip::~Ip()
{
}

bool Ip::isUdp() const
{
    return false;
}

} // namespace onvifxx
