#include <onvifxx/remotediscovery.h>
#include <soapRemoteDiscoveryBindingService.h>


namespace onvifxx {

struct RemoteDiscoveryService : RemoteDiscovery, RemoteDiscoveryBindingService
{
    virtual	RemoteDiscoveryBindingService * copy()
    {
        return new RemoteDiscoveryService;
    }

    virtual	int Hello(wsdd__HelloType * wsdd__Hello, wsdd__ResolveType * wsdd__HelloResponse)
    {
        wsdd__Hello = 0;
        wsdd__HelloResponse = 0;

        return 0;
    }

    virtual	int Bye(wsdd__ByeType * wsdd__Bye, wsdd__ResolveType * wsdd__ByeResponse)
    {
        wsdd__Bye = 0;
        wsdd__ByeResponse = 0;

        return 0;
    }

    virtual ResolveType hello(HelloType hello)
    {
        (void)hello;

        return ResolveType();
    }

    virtual ResolveType bye(ByeType bye)
    {
        (void)bye;

        return ResolveType();
    }
};

template<>
RemoteDiscovery * createService<RemoteDiscovery>()
{
    return new RemoteDiscoveryService;
}



} // namespace onvifxx
