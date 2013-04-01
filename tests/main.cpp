#include <onvifxx/discovery.hpp>
//#include <onvifxx/media.hpp>
#include <unistd.h>

struct DiscoveryService : onvifxx::DiscoveryServer
{
    virtual ResolveType hello(HelloType arg)
    {
        return onvifxx::DiscoveryServer::hello(arg);
    }

    virtual ResolveType bye(ByeType arg)
    {
        return onvifxx::DiscoveryServer::bye(arg);
    }

    virtual ProbeMatchesType probe(ProbeType arg)
    {
        return onvifxx::DiscoveryServer::probe(arg);
    }

};

int main(int argc, char ** argv)
{
    //DiscoveryService ds;
    onvifxx::DiscoveryClient dc;
    dc.probe(onvifxx::DiscoveryLookup::ProbeType());

    while (true)
        usleep(1000);
//    auto m = onvifxx::createMediaClient();
//    m->getServiceCapabilities();
//    delete m;

    return 0;
}
