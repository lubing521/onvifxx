#include <onvifxx/discovery.hpp>
//#include <onvifxx/media.hpp>

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>


namespace asio = boost::asio;
typedef asio::ip::udp AsioUdp_t;


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
    try {
        onvifxx::DiscoveryClient dc;
        dc.probe(onvifxx::DiscoveryLookup::ProbeType());

//        boost::asio::io_service io_service;

//        // We use an boost::asio::ip::udp::resolver object to find the correct remote endpoint to use based on the host and service names.
//        // The query is restricted to return only IPv4 endpoints by the boost::asio::ip::udp::v4() argument.
//        AsioUdp_t::resolver resolver(io_service);
//        AsioUdp_t::resolver::query query(AsioUdp_t::v4(), "239.255.255.250", "ws-discovery");

//        // The boost::asio::ip::udp::resolver::resolve() function is guaranteed to return at least one endpoint in the list if it does not fail.
//        // This means it is safe to dereference the return value directly.
//        AsioUdp_t::endpoint receiver_endpoint = *resolver.resolve(query);

//        // Since UDP is datagram-oriented, we will not be using a stream socket.
//        // Create an boost::asio::ip::udp::socket and initiate contact with the remote endpoint.
//        AsioUdp_t::socket socket(io_service);
//        socket.open(AsioUdp_t::v4());

//        std::string send_buf  = \
//                //"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\ xmlns:a=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">"
//                "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">"
//                "<s:Header><a:Action s:mustUnderstand=\"1\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</a:Action><a:MessageID>uuid:3db95a20-e1ff-429e-aad5-102d185b5a5e</a:MessageID><a:ReplyTo><a:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</a:Address></a:ReplyTo><a:To s:mustUnderstand=\"1\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</a:To></s:Header>"
//                "<s:Body><Probe xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"><d:Types xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dp0=\"http://www.onvif.org/ver10/network/wsdl\">dp0:NetworkVideoTransmitter</d:Types></Probe></s:Body>"
//                "</s:Envelope>";
//        socket.send_to(asio::buffer(send_buf), receiver_endpoint);

//        // Now we need to be ready to accept whatever the server sends back to us.
//        // The endpoint on our side that receives the server's response will be initialised by boost::asio::ip::udp::socket::receive_from().
//        boost::array<char, 65536> recv_buf;
//        AsioUdp_t::endpoint sender_endpoint;
//        size_t len = 0;
//        do {
//            len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
//            std::cerr.write(recv_buf.data(), len);
//            std::cerr << std::endl << len;
//        } while (len != 0);
    } catch (const onvifxx::UnixException & ex) {
        std::cerr << "Error (" << ex.code() << ") " << ex.what() << "!" << std::endl;
    } catch (const std::exception & ex) {
        std::cerr << "Error (" << ex.what() << ")!" << std::endl;
    }



    usleep(1000);
//    auto m = onvifxx::createMediaClient();
//    m->getServiceCapabilities();
//    delete m;

    return 0;
}
