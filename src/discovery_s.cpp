#include <onvifxx/discovery.hpp>
#include "../wsdd/wsdd.hpp"

namespace onvifxx {
namespace server {

RemoteDiscovery::ResolveType Discovery::hello(RemoteDiscovery::HelloType arg)
{
    return RemoteDiscovery::ResolveType();
}

RemoteDiscovery::ResolveType Discovery::bye(RemoteDiscovery::ByeType arg)
{
    return RemoteDiscovery::ResolveType();
}

DiscoveryLookup::ProbeMatch::List_t Discovery::probe(Probe probe)
{
    return DiscoveryLookup::ProbeMatch::List_t();
}


} // namespace server
} // namespace onvifxx
