#include <onvif/remotediscovery.h>
#include <soapRemoteDiscoveryBindingProxy.h>
#include <soap.nsmap>

namespace onvif {

struct RemoteDiscoveryImpl : RemoteDiscoveryBindingProxy, RemoteDiscovery
{

};

RemoteDiscovery::RemoteDiscovery()
{
}

RemoteDiscovery::ResolveType RemoteDiscovery::Hello(RemoteDiscovery::HelloType hello)
{
    return ResolveType();
}

RemoteDiscovery::ResolveType RemoteDiscovery::Bye(RemoteDiscovery::ByeType  bye)
{
    return ResolveType();
}

RemoteDiscovery * RemoteDiscovery::create()
{
    return new RemoteDiscovery;
}

} // namespace onvif
