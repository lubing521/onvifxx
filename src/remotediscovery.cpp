#include <onvifxx/remotediscovery.h>
#include <soapRemoteDiscoveryBindingProxy.h>
#include <soapRemoteDiscoveryBindingService.h>
#include <soap.nsmap>

namespace onvif {

struct RemoteDiscoveryClient : RemoteDiscovery, RemoteDiscoveryBindingProxy
{
    virtual ResolveType Hello(HelloType hello)
    {
        ns22__HelloType * ht = 0;
        ns22__ResolveType * rt = 0;

        int n = RemoteDiscoveryBindingProxy::Hello(ht,  rt);
        if (n != 0)
            throw std::exception();

        return ResolveType();
    }

};


struct RemoteDiscoveryServer : RemoteDiscovery, RemoteDiscoveryBindingService
{
    virtual ResolveType Hello(HelloType hello)
    {
        ns22__HelloType * ht = 0;
        ns22__ResolveType * rt = 0;

        int n = Hello(ht,  rt);
        if (n != 0)
            throw std::exception();

        return ResolveType();
    }

    virtual	RemoteDiscoveryBindingService * copy()
    {
        return new RemoteDiscoveryServer;
    }

    virtual	int Hello(ns22__HelloType * ns21__Hello, ns22__ResolveType * ns21__HelloResponse)
    {
        ns21__Hello = 0;
        ns21__HelloResponse = 0;

        return 0;
    }

    virtual	int Bye(ns22__ByeType * ns21__Bye, ns22__ResolveType * ns21__ByeResponse)
    {
        ns21__Bye = 0;
        ns21__ByeResponse = 0;

        return 0;
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

RemoteDiscovery * RemoteDiscovery::client()
{
    return new RemoteDiscoveryClient;
}

RemoteDiscovery * RemoteDiscovery::server()
{
    return new RemoteDiscoveryServer;
}


} // namespace onvif
