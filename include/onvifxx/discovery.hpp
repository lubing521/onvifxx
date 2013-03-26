#ifndef ONVIF_REMOTEDICOVERY_H
#define ONVIF_REMOTEDICOVERY_H

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

class DiscoveryClient :
        protected RemoteDiscovery,
        public DiscoveryLookup
{
public:
    virtual ProbeMatchesType probe(ProbeType arg);

protected:
    virtual ResolveType hello(HelloType arg);
    virtual ResolveType bye(ByeType arg);

};

class DiscoveryServer :
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

#endif // ONVIF_REMOTEDICOVERY_H
