#include <onvifxx/discovery.hpp>
#include "wsdd.h"
//#include <boost/bind.hpp>
//#include <boost/function.hpp>

using namespace std::placeholders;

namespace onvifxx {

struct DiscoveryClient::Impl : EventHandlerMap
{
    soap * c;
    soap * s;

    static void onHello(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageID, const char * relatesTo, const char * endpointReference, const char * types, const char * scopes, const char * matchBy, const char * xAddrs, unsigned metadataVersion)
    {
    }

    static void onBye(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageID, const char * relatesTo, const char * endpointReference, const char * types, const char * scopes, const char * matchBy, const char * xAddrs, unsigned int * metadataVersion)
    {

    }

    static soap_wsdd_mode onProbe(soap * s, const char * messageId, const char * replyTo, const char * types, const char * scopes, const char * matchBy, wsdd__ProbeMatchesType * matches)
    {
        return SOAP_WSDD_ADHOC;
    }

    static void onProbeMatches(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageId, const char * relatesTo, wsdd__ProbeMatchesType * matches)
    {

    }

    static soap_wsdd_mode onResolve(soap * s, const char * messageId, const char * replyTo, const char * endpointReference, struct wsdd__ResolveMatchType * match)
    {
        return SOAP_WSDD_ADHOC;
    }

    static void onResolveMatches(soap * s, unsigned instanceId, const char * sequenceId, unsigned messageNumber, const char * messageId, const char * relatesTo, wsdd__ResolveMatchType * match)
    {

    }
};

DiscoveryClient::DiscoveryClient() :
    impl_(new Impl)
{
    impl_->c = soap_new();
    impl_->s = soap_new();

    impl_->c->user = impl_.get();
    impl_->s->user = impl_.get();

    impl_->hello            = Impl::onHello;
    impl_->bye              = Impl::onBye;
    impl_->probe            = Impl::onProbe;
    impl_->probeMatches     = Impl::onProbeMatches;
    impl_->resolve          = Impl::onResolve;
    impl_->resolveMatches   = Impl::onResolveMatches;

    impl_->c->connect_flags = SO_BROADCAST;
    soap_bind(impl_->s, "", 3702, 100);
}

DiscoveryLookup::ProbeMatchesType DiscoveryClient::probe(DiscoveryLookup::ProbeType arg)
{
    const char * id = soap_wsa_rand_uuid(impl_->c);
    soap_wsdd_Probe(impl_->c, SOAP_WSDD_ADHOC, SOAP_WSDD_TO_TS,
        "soap.udp://239.255.255.250", id, 0, 0, 0, 0);


    soap_wsdd_listen(impl_->s, -1000);

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
