#include <onvifxx/discovery.hpp>
#include "../wsdd/wsdd.h"

namespace onvifxx {

struct DiscoveryServer::Impl : EventHandlerMap
{
    soap * c;
    soap * s;

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

DiscoveryServer::DiscoveryServer() :
    impl_(new Impl)
{
//    impl_->c = soap_new();
//    impl_->s = soap_new();

//    impl_->c->user = impl_.get();
//    impl_->s->user = impl_.get();

//    impl_->hello            = Impl::onHello;
//    impl_->bye              = Impl::onBye;
//    impl_->probe            = Impl::onProbe;
//    impl_->probeMatches     = Impl::onProbeMatches;
//    impl_->resolve          = Impl::onResolve;
//    impl_->resolveMatches   = Impl::onResolveMatches;

//    impl_->c->connect_flags = SO_BROADCAST;
//    soap_bind(impl_->s, "127.0.0.1", 3702, 100);
}

RemoteDiscovery::ResolveType DiscoveryServer::hello(RemoteDiscovery::HelloType arg)
{
    return RemoteDiscovery::ResolveType();
}

RemoteDiscovery::ResolveType DiscoveryServer::bye(RemoteDiscovery::ByeType arg)
{
    return RemoteDiscovery::ResolveType();
}

DiscoveryLookup::ProbeMatchesType DiscoveryServer::probe(DiscoveryLookup::ProbeType arg)
{
    return DiscoveryLookup::ProbeMatchesType();
}

} // namespace onvifxx
