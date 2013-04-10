#ifndef ONVIF_DISCOVERY_H
#define ONVIF_DISCOVERY_H

#include "onvifxx.hpp"

namespace onvifxx {

struct DiscoveryLookup
{
    struct Probe { };
    struct ProbeMatch
    {
        typedef std::vector<ProbeMatch> List_t;
    };

    virtual void probe(Probe) = 0;
    virtual void getProbeMatches(ProbeMatch::List_t &) = 0;
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

    virtual void probe(Probe);
    virtual void getProbeMatches(ProbeMatch::List_t &);

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
    virtual void probe(Probe);
    virtual void getProbeMatches(ProbeMatch::List_t &);
};

} // namespace server

} // namespace onvifxx

#endif // ONVIF_DISCOVERY_H
