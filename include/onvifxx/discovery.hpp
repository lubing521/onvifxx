#ifndef ONVIF_DISCOVERY_H
#define ONVIF_DISCOVERY_H

#include "onvifxx.hpp"

namespace onvifxx {

struct DiscoveryLookup
{
    struct ProbeType { };
    struct ProbeMatchesType { };

    virtual ProbeMatchesType probe(ProbeType arg) = 0;
};

struct RemoteDiscovery
{
    struct HelloType { };
    struct ByeType { };
    struct ResolveType { };

    virtual ResolveType hello(HelloType arg) = 0;
    virtual ResolveType bye(ByeType arg) = 0;
};

class DiscoveryClientImpl;
class DiscoveryClient :
        virtual private Pimpl<DiscoveryClientImpl>,
        protected RemoteDiscovery,
        public DiscoveryLookup
{
public:
    DiscoveryClient();
    virtual ProbeMatchesType probe(ProbeType arg);

protected:
    virtual ResolveType hello(HelloType arg);
    virtual ResolveType bye(ByeType arg);

private:
    class Impl;
};

class DiscoveryServer :
        virtual private Pimpl<Server>,
        public RemoteDiscovery,
        protected DiscoveryLookup
{
public:
    virtual ResolveType hello(HelloType arg);
    virtual ResolveType bye(ByeType arg);

protected:
    virtual ProbeMatchesType probe(ProbeType arg);
};

} // namespace onvifxx

#endif // ONVIF_DISCOVERY_H
