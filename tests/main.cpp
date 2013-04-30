#include <onvifxx/discoverylookup.hpp>
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

struct DiscoveryLookup : onvifxx::DiscoveryLookup
{

};

int main(int argc, char ** argv)
{
    try {
        auto dls = onvifxx::Service<onvifxx::DiscoveryLookup>();
        dls.run();

        std::string types = "dn:NetworkVideoTransmitter";
        auto dlp = onvifxx::proxy<onvifxx::DiscoveryLookup>();
        for (const auto & match : dlp->probe(&types, nullptr)) {
             std::cout << match << std::endl;
        }
    } catch (const onvifxx::UnixException & ex) {
        std::cerr << "Error (" << ex.code() << ") " << ex.what() << "!" << std::endl;
        return 1;
    } catch (const std::exception & ex) {
        std::cerr << "Error (" << ex.what() << ")!" << std::endl;
        return 2;
    }

    return 0;
}
