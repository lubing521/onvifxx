#include <onvifxx/discovery.hpp>
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

int main(int argc, char ** argv)
{
    try {
        onvifxx::client::Discovery dc;
        onvifxx::DiscoveryLookup::Probe probe;
        probe.type = "dn:NetworkVideoTransmitter";
        for (const auto & probeMatch : dc.probe(probe)) {
             std::cout << probeMatch.endpoint.first << ":" << probeMatch.endpoint.second << std::endl;
             std::cout << probeMatch.probe.type << " " << probeMatch.probe.scope.item << " "  << probeMatch.probe.scope.matchBy << std::endl;
             std::cout << probeMatch.xaddr << std::endl;
             std::cout << probeMatch.version << std::endl;
             std::cout << std::endl;
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
