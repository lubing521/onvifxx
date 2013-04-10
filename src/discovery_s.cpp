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

void Discovery::probe(Probe probe)
{

}

void Discovery::getProbeMatches(ProbeMatch::List_t & probeMatches)
{

}

} // namespace server
} // namespace onvifxx
