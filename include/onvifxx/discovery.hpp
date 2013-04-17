#ifndef ONVIF_DISCOVERY_H
#define ONVIF_DISCOVERY_H

#include "onvifxx.hpp"

namespace onvifxx {

struct DiscoveryLookup
{
    struct Probe
    {
        std::string type;
        Scope scope;
    };

    struct ProbeMatch
    {
        typedef std::pair<std::string, unsigned short> Endpoint_t;

        Endpoint_t endpoint;
        Probe probe;
        std::string xaddr;
        uint version;

        typedef std::vector<ProbeMatch> List_t;
    };

    virtual ProbeMatch::List_t probe(Probe) = 0;
};

struct RemoteDiscovery
{
    struct HelloType { };
    struct ByeType { };
    struct ResolveType { };

    virtual ResolveType hello(HelloType arg) = 0;
    virtual ResolveType bye(ByeType arg) = 0;
};

namespace client {

class Discovery :
        protected RemoteDiscovery,
        public DiscoveryLookup
{
public:
    Discovery();
    virtual ~Discovery();

    virtual ProbeMatch::List_t probe(Probe);

protected:
    virtual ResolveType hello(HelloType arg);
    virtual ResolveType bye(ByeType arg);

private:
    struct Impl;
    Impl * impl_;
};

} // namespace client

namespace server {

class Discovery :
        public RemoteDiscovery,
        protected DiscoveryLookup
{
public:
    virtual ResolveType hello(HelloType arg);
    virtual ResolveType bye(ByeType arg);

protected:
    virtual ProbeMatch::List_t probe(Probe);
};

} // namespace server

} // namespace onvifxx

#endif // ONVIF_DISCOVERY_H
