#include <onvifxx/remotediscovery.hpp>
#include <WsddRemoteDiscoveryBindingService.h>
#include "wsa.hpp"

#include <boost/assert.hpp>

namespace onvifxx {

const std::string TO_TS_URL = "schemas-xmlsoap-org:ws:2005:04:discovery";

class RemoteDiscoveryService :
    public BaseService<RemoteDiscovery, RemoteDiscoveryBindingService>
{
    static const uint TIMEOUT = 5; // second
    static const uint APP_MAX_DELAY = 500;

public:
    RemoteDiscoveryService() :
        wsa_(this)
    {
        this->send_timeout = TIMEOUT;
        this->recv_timeout = TIMEOUT;
        this->accept_timeout = TIMEOUT;

        endpoint_.soap_default(this);
    }

    virtual operator soap *()
    {
        return this;
    }

    virtual int bind(RemoteDiscovery * obj, int port)
    {
        this->p = obj;
        if (port == 0)
            port = 3702;

        soap_mode(this, SOAP_IO_UDP | SOAP_XML_IGNORENS);

        int socket = RemoteDiscoveryBindingService::bind(nullptr, port, 100);
        if (!soap_valid_socket(socket))
            return SOAP_INVALID_SOCKET;

        ip_mreq mcast;
        mcast.imr_multiaddr.s_addr = inet_addr(WSDD_MULTICAT_IP);
        mcast.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mcast, sizeof(mcast)) < 0)
            return SOAP_INVALID_SOCKET;

        return socket;
    }

    virtual	int dispatch()
    {
        try {
            int rv = RemoteDiscoveryBindingService::dispatch();
            if (rv == SOAP_NO_METHOD)
                return SOAP_OK;
        } catch (std::exception & ex) {
            RemoteDiscoveryBindingService::soap_senderfault("RemoteDiscovery", ex.what(), nullptr);
            return SOAP_FAULT;
        }
    }

    virtual RemoteDiscoveryBindingService * copy()
    {
        return new RemoteDiscoveryService;
    }

    virtual int Hello(wsd__HelloType * dn__Hello, wsd__ResolveType * dn__HelloResponse)
    {
        BOOST_ASSERT(dn__Hello != nullptr && dn__HelloResponse != nullptr);

        if (p != nullptr) {
            wsd__ScopesType wsdScopes;
            if (dn__Hello->Scopes != nullptr)
                wsdScopes = *dn__Hello->Scopes;
            RemoteDiscovery::Scopes_t scopes(wsdScopes.__item, wsdScopes.MatchBy);
            p->hello(dn__Hello->XAddrs, dn__Hello->Types, &scopes);
        }

        dn__HelloResponse->wsa__EndpointReference = &endpoint_;
        return SOAP_OK;
    }

    virtual int Bye(wsd__ByeType * dn__Bye, wsd__ResolveType * dn__ByeResponse)
    {
        BOOST_ASSERT(dn__Bye != nullptr && dn__ByeResponse != nullptr);

        if (p != nullptr) {
            wsd__ScopesType wsdScopes;
            if (dn__Bye->Scopes != nullptr)
                wsdScopes = *dn__Bye->Scopes;
            RemoteDiscovery::Scopes_t scopes(wsdScopes.__item, wsdScopes.MatchBy);
            p->bye(dn__Bye->XAddrs, dn__Bye->Types, &scopes);
        }

        dn__ByeResponse->wsa__EndpointReference = &endpoint_;
        return SOAP_OK;
    }

    virtual int Probe(wsd__ProbeType * dn__Probe, wsd__ProbeMatchesType * dn__ProbeResponse)
    {
        BOOST_ASSERT(dn__Probe != nullptr && dn__ProbeResponse != nullptr);

        if (p != nullptr) {
            wsd__ScopesType wsdScopes;
            if (dn__Probe->Scopes != nullptr)
                wsdScopes = *dn__Probe->Scopes;
            RemoteDiscovery::Scopes_t scopes(wsdScopes.__item, wsdScopes.MatchBy);
            probes_ = p->probe(dn__Probe->Types, &scopes);
        }

        responce_.resize(probes_.size());
        dn__ProbeResponse->ProbeMatch.clear();
        dn__ProbeResponse->ProbeMatch.reserve(responce_.size());

        for (size_t i = 0; i < probes_.size(); ++i) {
            responce_[i].wsa__EndpointReference = &endpoint_;
            responce_[i].Types = dn__Probe->Types;
            responce_[i].Scopes = dn__Probe->Scopes;
            responce_[i].XAddrs = &probes_[i];
            responce_[i].MetadataVersion = 0;

            dn__ProbeResponse->ProbeMatch.push_back(&responce_[i]);
        }

        return SOAP_OK;
    }

private:
    Wsa wsa_;
    wsa__EndpointReferenceType endpoint_;
    std::vector<std::string> probes_;
    std::vector<wsd__ProbeMatchType> responce_;
};




//RemoteDiscovery::ResolveType Discovery::hello(Hello arg)
//{
//    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/Hello";

//    const auto messageId = impl_->genUuid();

//    // SOAP Header
//    const std::string & dst = TO_TS_URL;
//    soap_wsa_request(impl_, messageId.c_str(), dst.c_str(), ACTION.c_str());
//    soap_wsa_add_RelatesTo(impl_, "");

//    impl_->resetAppSequence();

//    // Hello
//    wsd__HelloType req;
//    soap_default_wsd__HelloType(impl_, &req);
//    req.wsa__EndpointReference.Address = const_cast<char *>(arg.endpoint.c_str());
//    req.Types = const_cast<char *>(arg.type.c_str());

//    wsd__ScopesType req_scopes;
//    if (!scopes.empty()) {
//        soap_default_wsd__ScopesType(impl_, &req_scopes);
//        req_scopes.__item = const_cast<char *>(scopes.c_str());
//        req_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
//        req.Scopes = &req_scopes;
//    }
//    req.XAddrs = const_cast<char *>(xaddrs.c_str());
//    req.MetadataVersion = metadataVersion;

//    if (impl_->send_Hello(endpoint.c_str(), ACTION.c_str(), &req) != 0)
//        throw SoapException(impl_);

//    return RemoteDiscovery::ResolveType();
//}

//RemoteDiscovery::ResolveType Discovery::bye(ByeType)
//{
//    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/Bye";

//    // SOAP Header
//    const std::string & dst = mode == MODE_ADHOC ? endpoint : TO_TS_URL;
//    soap_wsa_request(impl_, messageId.c_str(), dst.c_str(), ACTION.c_str());
//#ifdef SOAP_WSA_2005
//    impl_->setAppSequence();
//#else
//    if (mode == MODE_ADHOC)
//        impl_->setAppSequence();
//    else
//        impl_->resetAppSequence();
//#endif

//    // Bye
//    wsd__ByeType req;
//    soap_default_wsd__ByeType(impl_, &req);
//#ifdef SOAP_WSA_2005
//    req.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#else
//    req.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#endif
//    req.Types = const_cast<char *>(types.c_str());

//    wsd__ScopesType req_scopes;
//    if (!scopes.empty()) {
//        soap_default_wsd__ScopesType(impl_, &req_scopes);
//        req_scopes.__item = const_cast<char *>(scopes.c_str());
//        req_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
//        req.Scopes = &req_scopes;
//    }
//    req.XAddrs = const_cast<char *>(xaddrs.c_str());
//    req.MetadataVersion = &metadataVersion;

//    if (impl_->send_Bye(endpoint.c_str(), ACTION.c_str(), &req) != 0)
//        throw SoapException(impl_);

//    return RemoteDiscovery::ResolveType();
//}


//void Discovery::resolve(ResolveType)
//{
//    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/Resolve";

//    // SOAP Header
//    const std::string & dst = to == TO_TS ? endpoint : TO_TS_URL;
//    soap_wsa_request(impl_, messageId.c_str(), dst.c_str(), ACTION.c_str());
//    soap_wsa_add_ReplyTo(impl_, replyTo.c_str());
//    impl_->resetAppSequence();

//    /* Probe */
//    wsd__ResolveType req;
//    soap_default_wsd__ResolveType(impl_, &req);
//#ifdef SOAP_WSA_2005
//    req.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#else
//    req.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#endif

//    if (impl_->send_Resolve(endpoint.c_str(), ACTION.c_str(), &req) != 0)
//        throw SoapException(impl_);
//}

//void Discovery::probeMatches(const ProbeMatch::List_t & matches)
//{
//    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/ProbeMatches";

//    // 0..APP_MAX_DELAY ms delay
//    impl_->delay();

//    // SOAP Header
//    soap_wsa_request(impl_, messageId.c_str(), to.c_str(), ACTION.c_str());
//    soap_wsa_add_RelatesTo(impl_, relatesTo.c_str());
//    impl_->setAppSequence();

//    // ProbeMatches
//    ProbeMatches probe_matches(matches);
//    if (impl_->send_ProbeMatches(endpoint.c_str(), ACTION.c_str(), &probe_matches) != 0)
//        throw SoapException(impl_);


//}

//void Discovery::resolveMatches(const std::string & endpoint, const std::string & messageId,
//                          const std::string & relatesTo, const std::string & to,
//                          const std::string & endpointRef, const std::string & types,
//                          const std::string & scopes, const std::string & matchBy,
//                          const std::string & xaddrs, uint metadataVersion)
//{
//    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/ResolveMatches";

//    // 0..APP_MAX_DELAY ms delay
//    impl_->delay();

//    // SOAP Header
//    soap_wsa_request(impl_, messageId.c_str(), to.c_str(), ACTION.c_str());
//    soap_wsa_add_RelatesTo(impl_, relatesTo.c_str());
//    impl_->setAppSequence();

//    // ResolveMatches
//    wsd__ResolveMatchType match;
//    soap_default_wsd__ResolveMatchType(impl_, &match);
//#ifdef SOAP_WSA_2005
//    match.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#else
//    match.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#endif
//    match.Types = const_cast<char *>(types.c_str());

//    wsd__ScopesType match_scopes;
//    if (!scopes.empty()) {
//      soap_default_wsd__ScopesType(impl_, &match_scopes);
//      match_scopes.__item = const_cast<char *>(scopes.c_str());
//      match_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
//      match.Scopes = &match_scopes;
//    }
//    match.XAddrs = const_cast<char *>(xaddrs.c_str());
//    match.MetadataVersion = metadataVersion;


//    wsd__ResolveMatchesType res;
//    soap_default_wsd__ResolveMatchesType(impl_, &res);
//    res.ResolveMatch = &match;
//    if (impl_->send_ResolveMatches(endpoint.c_str(), ACTION.c_str(), &res) != 0)
//        throw SoapException(impl_);


//}


//Discovery::ProbeMatch::List_t Discovery::getProbeMatches()
//{
//    Discovery::ProbeMatch::List_t rv;

//    // managed mode: receive the matches
//    struct __wsd__ProbeMatches res;
//    wsd__ProbeMatchesType probe_matches;
//    soap_default_wsd__ProbeMatchesType(impl_, &probe_matches);
//    res.wsd__ProbeMatches = &probe_matches;

//    if (impl_->recv_ProbeMatches(res) != 0)
//        throw SoapException(impl_);

//    impl_->checkHeader("ProbeMatches wrong header");
//    if (res.wsd__ProbeMatches == nullptr) {
//        if (soap_wsa_sender_fault(impl_, "WSDD ProbeMatches incomplete", nullptr) == 0)
//            throw SoapException(impl_);
//    }

//    for (int i = 0; i < res.wsd__ProbeMatches->__sizeProbeMatch; ++i) {
//        auto & probe_match = res.wsd__ProbeMatches->ProbeMatch[i];
//        rv.push_back(ProbeMatch());

//        rv.back().endpoint.first    = Impl::toString(probe_match.wsa__EndpointReference.Address);
//        rv.back().types             = Impl::toString(probe_match.Types);
//        rv.back().scopes.item       = Impl::toString(probe_match.Scopes->__item);
//        rv.back().scopes.matchBy    = Impl::toString(probe_match.Scopes->MatchBy);
//        rv.back().xaddrs            = Impl::toString(probe_match.XAddrs);
//        rv.back().version           = probe_match.MetadataVersion;
//    }

//    return rv;
//}

//Discovery::ResolveMatch Discovery::getResolveMatch()
//{
//    ResolveMatch rv;

//    // managed mode: receive the matches
//    struct __wsd__ResolveMatches res;
//    soap_default_wsd__ResolveMatchesType(impl_, res.wsd__ResolveMatches);
//    if (impl_->recv_ResolveMatches(res) != 0)
//        throw SoapException(impl_);

//    impl_->checkHeader("WSDD ResolveMatches header incomplete");
//    if (res.wsd__ResolveMatches == nullptr || res.wsd__ResolveMatches->ResolveMatch == nullptr)
//        if (soap_wsa_sender_fault(impl_, "WSDD ResolveMatches incomplete", nullptr) == 0)
//            throw SoapException(impl_);

//    auto resolve_match = res.wsd__ResolveMatches->ResolveMatch;
//    rv.endpoint.first = resolve_match->wsa__EndpointReference.Address;
//    rv.types = resolve_match->Types;
//    rv.scopes.item = resolve_match->Scopes->__item;
//    rv.scopes.matchBy = resolve_match->Scopes->MatchBy;
//    rv.xaddrs = resolve_match->XAddrs;
//    rv.version = resolve_match->MetadataVersion;

//    return rv;
//}

Service<RemoteDiscovery> * RemoteDiscovery::service()
{
    return new RemoteDiscoveryService;
}

} // namespace onvifxx

//int Discovery::listen(int timeout)
//{
//    impl_->accept_timeout = impl_->recv_timeout = impl_->send_timeout = timeout;

//    // event-serve loop (exits upon timeout)
//    for (;;) {
//      if (!soap_valid_socket(soap_accept(impl_))) {
//        // timeout?
//        if (impl_->errnum == 0)
//          return impl_->error = SOAP_OK;

//        return impl_->error;
//      }

//      if (soap_begin_serve(impl_))
//        return impl_->error;

//      /* always close HTTP afterwards */
//      impl_->keep_alive = 0;

//      /* receive event and serve */
//      if (soap_wsdd_serve_request(impl_) != 0) {
//        // timeout?
//        if (!impl_->errnum)
//          return impl_->error = SOAP_OK;

//#ifdef WITH_FASTCGI
//        soap_send_fault(impl_);
//#else
//        return soap_send_fault(impl_);
//#endif
//      }

//      // clean up for next iteration
//      soap_destroy(impl_);
//      soap_end(impl_);
//    }
//}
