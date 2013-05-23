#ifndef ONVIF_DISCOVERY_HPP
#define ONVIF_DISCOVERY_HPP

#include "onvifxx.hpp"

namespace onvifxx {


struct RemoteDiscovery
{
    typedef std::pair<std::string, std::string *> Scopes_t;
    typedef std::vector<std::string> ProbeMatches_t;

    virtual void hello() = 0;
    virtual void bye() = 0;
    virtual ProbeMatches_t probe(std::string * types, Scopes_t * scopes) = 0;

    static Proxy<RemoteDiscovery> * createProxy();
    static Service<RemoteDiscovery> * createService();
};


} // namespace onvifxx

#endif // ONVIF_DISCOVERY_HPP
