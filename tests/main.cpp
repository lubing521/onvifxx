#include <onvifxx/remotediscovery.hpp>
//#include <onvifxx/media.hpp>

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>


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
        std::cerr << "hello" << std::endl;
    }

    virtual void bye()
    {
        std::cerr << "bye" << std::endl;
    }

    virtual ProbeMatches_t probe(std::string * types, Scopes_t * scopes)
    {
        std::cerr << "probe" << std::endl;
        return ProbeMatches_t();
    }
};

typedef onvifxx::Proxy<onvifxx::RemoteDiscovery> Proxy_t;
typedef onvifxx::Service<onvifxx::RemoteDiscovery> Service_t;


int main(int argc, char ** argv)
{
    try {
        RemoteDiscoveryService obj;
        boost::scoped_ptr<Service_t> service(onvifxx::RemoteDiscovery::service());
        BOOST_ASSERT(service->bind(&obj) != -1);
        for (int i = 0; i < 100000; ++i) {
            std::cerr << i << std::endl;

            int socket = service->accept();
            BOOST_ASSERT(socket != -1);

            int error = service->serve();
            if (error == -1)
                continue;
            if (error != 0)
                throw onvifxx::SoapException(*service);

            service->destroy();
        }

        std::string types = "dn:NetworkVideoTransmitter";
        boost::scoped_ptr<Proxy_t>  proxy(onvifxx::RemoteDiscovery::proxy());
        onvifxx::RemoteDiscovery::ProbeMatches_t matches = proxy->probe(&types, nullptr);
        std::copy(matches.begin(), matches.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

    } catch (const onvifxx::UnixException & ex) {
        std::cerr << "Error (" << ex.code() << ") " << ex.what() << "!" << std::endl;
        return 1;
    } catch (const std::exception & ex) {
        std::cerr << "Error (" << ex.what() << ")!" << std::endl;
        return 2;
    }

    return 0;
}
