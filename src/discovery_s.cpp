#include <onvifxx/discovery.hpp>
#include "../wsdd/wsdd.h"

namespace onvifxx {

RemoteDiscovery::ResolveType DiscoveryServer::hello(RemoteDiscovery::HelloType arg)
{
    return RemoteDiscovery::ResolveType();
}

RemoteDiscovery::ResolveType DiscoveryServer::bye(RemoteDiscovery::ByeType arg)
{
    return RemoteDiscovery::ResolveType();
}

DiscoveryLookup::ProbeMatchesType DiscoveryServer::probe(DiscoveryLookup::ProbeType arg)
{
    return DiscoveryLookup::ProbeMatchesType();
}

} // namespace onvifxx
