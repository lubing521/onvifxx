#include <onvifxx/discovery.hpp>
#include "../wsdd/wsdd.h"

namespace onvifxx {

struct DiscoveryClient::Impl
{
    soap sc;
    EventHandlerMap ehm;

    ~Impl()
    {
        soap_destroy(&sc);
        soap_end(&sc);
        soap_done(&sc);
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
};

DiscoveryClient::DiscoveryClient() :
    impl_(new Impl)
{
    //impl_->sc = soap_new1(SOAP_IO_DEFAULT | SOAP_IO_UDP);
    soap_init(&impl_->sc);

    impl_->ehm.hello           = Impl::onHello;
    impl_->ehm.bye             = Impl::onBye;
    impl_->ehm.probe           = Impl::onProbe;
    impl_->ehm.probeMatches    = Impl::onProbeMatches;
    impl_->ehm.resolve         = Impl::onResolve;
    impl_->ehm.resolveMatches  = Impl::onResolveMatches;

    soap_set_namespaces(&impl_->sc, wsdd_namespaces());
    impl_->sc.user          = &impl_->ehm;
    //impl_->sc.connect_flags = SO_BROADCAST;
}

DiscoveryLookup::ProbeMatchesType DiscoveryClient::probe(DiscoveryLookup::ProbeType arg)
{
    const char * id = soap_wsa_rand_uuid(&impl_->sc);
    int result = soap_wsdd_Probe(&impl_->sc, SOAP_WSDD_ADHOC, SOAP_WSDD_TO_TS,
                                 "soap.udp://239.255.255.250:3702", id, nullptr, "dn:NetworkVideoTransmitter", nullptr, nullptr);

    struct __wsdd__ProbeMatches res;
    int r = soap_recv___wsdd__ProbeMatches(&impl_->sc, &res);
    if (r != SOAP_OK) {
        std::cerr << r << std::endl;
        soap_print_fault(&impl_->sc, stderr);
    } else if (!impl_->sc.header) {
        soap_wsa_sender_fault(&impl_->sc, "WSDD ProbeMatches incomplete", NULL);
    } else if (wsdd__AppSequenceType * seq = impl_->sc.header->wsdd__AppSequence) {
        std::cout << "Seq: " << seq->InstanceId <<
                     " " << seq->InstanceId <<
                     " " << seq->MessageNumber << std::endl;
    }

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
