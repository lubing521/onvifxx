#ifndef ONVIF_DISCOVERY_H
#define ONVIF_DISCOVERY_H

#include "onvifxx.hpp"

namespace onvifxx {


struct DiscoveryLookup
{
    virtual std::vector<std::string> probe(const std::string &) = 0;
};

struct RemoteDiscovery
{
    virtual void hello() = 0;
    virtual void bye() = 0;
};


} // namespace onvifxx

#endif // ONVIF_DISCOVERY_H
