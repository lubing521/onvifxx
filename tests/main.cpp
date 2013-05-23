#include <onvifxx/remotediscovery.hpp>
//#include <onvifxx/media.hpp>

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>


namespace asio = boost::asio;
typedef asio::ip::udp AsioUdp_t;


//struct DiscoveryService : onvifxx::Discovery
//{
//    virtual ResolveType hello(HelloType arg)
//    {
//        return onvifxx::DiscoveryServer::hello(arg);
//    }

//    virtual ResolveType bye(ByeType arg)
//    {
//        return onvifxx::DiscoveryServer::bye(arg);
//    }

//    virtual ProbeMatchesType probe(ProbeType arg)
//    {
//        return onvifxx::DiscoveryServer::probe(arg);
//    }

//};

struct RemoteDiscoveryService : onvifxx::RemoteDiscovery
{
    virtual void hello()
    {

    }

    virtual void bye()
    {

    }

    virtual ProbeMatches_t probe(std::string * types, Scopes_t * scopes)
    {
        return ProbeMatches_t();
    }
};

int main(int argc, char ** argv)
{
    try {
        RemoteDiscoveryService obj;
        onvifxx::Service<onvifxx::RemoteDiscovery> * service = onvifxx::RemoteDiscovery::createService();
        service->bind(&obj);
        BOOST_ASSERT(service->accept() != -1);
        BOOST_ASSERT(!service->serve() != -1);

        std::string types = "dn:NetworkVideoTransmitter";
        onvifxx::Proxy<onvifxx::RemoteDiscovery> * proxy = onvifxx::RemoteDiscovery::createProxy();
        onvifxx::RemoteDiscovery::ProbeMatches_t matches = proxy->probe(&types, nullptr);
        std::copy(matches.begin(), matches.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
        delete proxy;

    } catch (const onvifxx::UnixException & ex) {
        std::cerr << "Error (" << ex.code() << ") " << ex.what() << "!" << std::endl;
        return 1;
    } catch (const std::exception & ex) {
        std::cerr << "Error (" << ex.what() << ")!" << std::endl;
        return 2;
    }

    return 0;
}
