#include <onvifxx/discovery.hpp>
#include "../wsdd/wsdd.h"

#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/array.hpp>

namespace onvifxx {

class DiscoveryClientImpl
{
public:
    DiscoveryClientImpl()
    {
        ehm_.hello           = onHello;
        ehm_.bye             = onBye;
        ehm_.probe           = onProbe;
        ehm_.probeMatches    = onProbeMatches;
        ehm_.resolve         = onResolve;
        ehm_.resolveMatches  = onResolveMatches;

        soap_init(&soap_);
        soap_set_namespaces(&soap_, wsdd_namespaces());
        soap_.user = this;
    }

    template<class T>
    static std::shared_ptr<typename T::Context> get(T * p)
    {
        return std::static_pointer_cast<typename T::Context>(p->context_);
    }

    static void onHello(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageID, const char * relatesTo, const char * endpointReference, const char * types, const char * scopes, const char * matchBy, const char * xAddrs, unsigned metadataVersion)
    {
        std::cerr << instanceId << std::endl;
    }

    static void onBye(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageID, const char * relatesTo, const char * endpointReference, const char * types, const char * scopes, const char * matchBy, const char * xAddrs, unsigned int * metadataVersion)
    {
        std::cerr << instanceId << std::endl;
    }

    static soap_wsdd_mode onProbe(soap * s, const char * messageId, const char * replyTo, const char * types, const char * scopes, const char * matchBy, wsdd__ProbeMatchesType * matches)
    {
        std::cerr << messageId << std::endl;
        return SOAP_WSDD_ADHOC;
    }

    static void onProbeMatches(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageId, const char * relatesTo, wsdd__ProbeMatchesType * matches)
    {
        std::cerr << instanceId << std::endl;
    }

    static soap_wsdd_mode onResolve(soap * s, const char * messageId, const char * replyTo, const char * endpointReference, struct wsdd__ResolveMatchType * match)
    {
        std::cerr << messageId << std::endl;
        return SOAP_WSDD_ADHOC;
    }

    static void onResolveMatches(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageId, const char * relatesTo, wsdd__ResolveMatchType * match)
    {
        std::cerr << instanceId << std::endl;
    }

    const char * wsaRandUuid()
    {
        return soap_wsa_rand_uuid(&soap_) + 4;
    }


    template <typename T, size_t N>
    static size_t sizeOf(const T(&)[N])
    {
        return N;
    }

    void wsddProbe(const char * id)
    {
        const auto result = soap_wsdd_Probe(&soap_, SOAP_WSDD_ADHOC, SOAP_WSDD_TO_TS,
                                            "soap.udp://239.255.255.250:3702", id, nullptr,
                                            "dn:NetworkVideoTransmitter", nullptr, nullptr);
        if (result != SOAP_OK)
            throw SoapException(&soap_);


        while (true) {
            boost::array<char, 1024> buf;
            const auto count = recvfrom(soap_.socket, buf.data(), buf.size(), 0, nullptr, nullptr);
            if (count < 0)
                throw UnixException();

            buf[count] = '\0';
            std::cerr.write(buf.data(), count);
        }
    }

private:
    soap soap_;
    EventHandlerMap ehm_;
    boost::asio::io_service ios;
};

DiscoveryClient::DiscoveryClient()
{
    BOOST_ASSERT(impl() != nullptr);
}

DiscoveryLookup::ProbeMatchesType DiscoveryClient::probe(DiscoveryLookup::ProbeType arg)
{
    const char * id = impl()->wsaRandUuid();
    std::cerr << id << std::endl;
    impl()->wsddProbe(id);

//    impl_->sc.recvfd = fd[1];
//    impl_->sc.socket = -1;

//    Context::socket socket(ios);
//    socket.open(Context::AsioUdp_t::v4());

//    AsioUdp_t::endpoint to = Context::resolveUdp("239.255.255.250", "ws-discovery");


//    std::cerr << ss.rdbuf();

//    asio::io_service ios;

//    BOOST_ASSERT(socket.is_open() == false);

//    boost::array<char, 65536> buffer;

//    //asio::streambuf buffer;
//    //socket.receive_from(buffer, sender);
//    size_t len = 0;
//    AsioUdp_t::endpoint sender;
//    do {

//        len = socket.receive_from(asio::buffer(buffer), sender);
//        std::cerr.write(buffer.data(), len);
//        std::cerr << std::endl << len << std::endl;
//    } while (len != 0);


//    struct __wsdd__ProbeMatches res;
//    result = soap_recv___wsdd__ProbeMatches(&impl_->sc, &res);
//    if (result != SOAP_OK)
//        throw Exception(&impl_->sc);

//    if (!impl_->sc.header) {
//        soap_wsa_sender_fault(&impl_->sc, "WSDD ProbeMatches incomplete", "No header");
//    } else if (wsdd__AppSequenceType * seq = impl_->sc.header->wsdd__AppSequence) {
//        std::cout << "Seq: " << seq->InstanceId <<
//                     " " << seq->InstanceId <<
//                     " " << seq->MessageNumber << std::endl;
//    }

    return DiscoveryLookup::ProbeMatchesType();
}


RemoteDiscovery::ResolveType DiscoveryClient::hello(RemoteDiscovery::HelloType arg)
{
    return RemoteDiscovery::ResolveType();
}

RemoteDiscovery::ResolveType DiscoveryClient::bye(RemoteDiscovery::ByeType arg)
{
    return RemoteDiscovery::ResolveType();
}


} // namespace onvifxx
