#include <onvif/remotediscovery.h>
#include <soapRemoteDiscoveryBindingProxy.h>
#include <soap.nsmap>

namespace onvif {

struct RemoteDiscoveryImpl : RemoteDiscoveryBindingProxy, RemoteDiscovery
{
    virtual ResolveType Hello(HelloType hello)
    {
        ns22__HelloType * ht = 0;
        ns22__ResolveType * rt = 0;

        int n = RemoteDiscoveryBindingProxy::Hello(ht,  rt);
        if (n == 0)
            throw std::exception();
        return ResolveType();
    }

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
