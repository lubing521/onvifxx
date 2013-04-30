#ifndef ONVIF_DISCOVERY_HPP
#define ONVIF_DISCOVERY_HPP

#include "onvifxx.hpp"

namespace onvifxx {


struct DiscoveryLookup
{
    typedef std::pair<std::string, std::string *> Scopes_t;
    virtual std::vector<std::string> probe(std::string * types, Scopes_t * scopes) = 0;
};

struct RemoteDiscovery
{
    virtual void hello() = 0;
    virtual void bye() = 0;
};

template<>
std::unique_ptr<DiscoveryLookup> proxy();
template<>
std::unique_ptr<DiscoveryLookup> service();


} // namespace onvifxx

#endif // ONVIF_DISCOVERY_HPP
