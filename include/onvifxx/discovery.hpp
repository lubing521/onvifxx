#ifndef ONVIF_DICOVERY_H
#define ONVIF_DICOVERY_H

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
    DiscoveryClient();

    virtual ProbeMatchesType probe(ProbeType arg);

protected:
    virtual ResolveType hello(HelloType arg);
    virtual ResolveType bye(ByeType arg);

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

class DiscoveryServer :
        public RemoteDiscovery,
        protected DiscoveryLookup
{
public:
    DiscoveryServer();

    virtual ResolveType hello(HelloType arg);
    virtual ResolveType bye(ByeType arg);

protected:
    virtual ProbeMatchesType probe(ProbeType arg);

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace onvifxx

#endif // ONVIF_DICOVERY_H
