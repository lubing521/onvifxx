#ifndef ONVIF_DISCOVERY_HPP
#define ONVIF_DISCOVERY_HPP

#include "onvifxx.hpp"

namespace onvifxx {

static const char WSDD_MULTICAT_IP[] = "239.255.255.250";
static const int WSDD_MULTICAT_PORT = 3702;

struct RemoteDiscovery
{
    typedef std::pair<std::string, std::string *> Scopes_t;
    typedef std::vector<std::string> ProbeMatches_t;

    virtual void hello(std::string * xaddrs, std::string * types, Scopes_t * scopes) = 0;
    virtual void bye(std::string * xaddrs, std::string * types, Scopes_t * scopes) = 0;
    virtual ProbeMatches_t probe(std::string * types, Scopes_t * scopes) = 0;

    static Proxy<RemoteDiscovery> * proxy();
    static Service<RemoteDiscovery> * service();
};


} // namespace onvifxx

#endif // ONVIF_DISCOVERY_HPP
