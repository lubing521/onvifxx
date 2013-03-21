#include <onvifxx/remotediscovery.h>
#include <soapRemoteDiscoveryBindingProxy.h>
#include <soap.nsmap>

namespace onvifxx {

struct RemoteDiscoveryProxy : RemoteDiscovery, RemoteDiscoveryBindingProxy
{
    virtual ResolveType hello(HelloType hello)
    {
        wsdd__HelloType * ht = 0;
        wsdd__ResolveType * rt = 0;

        int n = RemoteDiscoveryBindingProxy::Hello(ht,  rt);
        if (n != 0)
            throw std::exception();

        return ResolveType();
    }

    virtual ResolveType bye(ByeType bye)
    {
        wsdd__HelloType * ht = 0;
        wsdd__ResolveType * rt = 0;

        int n = RemoteDiscoveryBindingProxy::Hello(ht,  rt);
        if (n != 0)
            throw std::exception();

        return ResolveType();
    }

};

template<>
RemoteDiscovery * createProxy<RemoteDiscovery>()
{
    return new RemoteDiscoveryProxy;
}


} // namespace onvifxx
