#include <onvifxx/discovery.hpp>

namespace onvifxx {

DiscoveryLookup::ProbeMatchesType DiscoveryClient::probe(DiscoveryLookup::ProbeType arg)
{
    return DiscoveryLookup::ProbeMatchesType();
}


RemoteDiscovery::ResolveType DiscoveryClient::hello(RemoteDiscovery::HelloType arg)
{
    return RemoteDiscovery::ResolveType();
}

RemoteDiscovery::ResolveType DiscoveryClient::bye(RemoteDiscovery::ByeType arg)
{
    return RemoteDiscovery::ResolveType();
}


} // namespace onvifxx
