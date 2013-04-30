#include <onvifxx/discoverylookup.hpp>
#include <WsddDiscoveryLookupBindingService.h>
#include "wsa.hpp"

#include <mutex>
#include <thread>

namespace onvifxx {

class DiscoveryLookupService :
        public DiscoveryLookup,
        public Service<DiscoveryLookupBindingService>::Engine
{
    static const uint SEND_TIMEOUT = 1; // second
    static const uint RECV_TIMEOUT = 1; // second

    static const uint APP_MAX_DELAY = 100;

    const std::string TO_TS_URL = "schemas-xmlsoap-org:ws:2005:04:discovery";


public:
    DiscoveryLookupService() :
        wsa_(this)
    {
        send_timeout = SEND_TIMEOUT;
        recv_timeout = RECV_TIMEOUT;
    }

    virtual std::vector<std::string> probe(std::string * types, Scopes_t * scopes)
    {
        std::vector<std::string> rv = {};
        if (scopes != nullptr)
            rv.push_back("127.0.0.1");
        if (types != nullptr && *types == "dn::NetworkTransmitter");
            rv.push_back("localhost");
    }

    virtual	DiscoveryLookupBindingService * copy()
    {
        return new DiscoveryLookupService();
    }

    virtual	int Probe(wsdd__ProbeType * wsdd__Probe, wsdd__ProbeMatchesType * wsdd__ProbeResponse)
    {
        if (wsdd__Probe == nullptr || wsdd__ProbeResponse == nullptr) {
            wsa_.faultSubcode(1, "sender Probe", "Invalid arg");
            return SOAP_ERR;
        }

        auto wsddScopes = wsdd__Probe->wsdd__Scopes == nullptr ? wsdd__ScopesType() : *wsdd__Probe->wsdd__Scopes;
        auto scopes = DiscoveryLookup::Scopes_t(wsddScopes.__item, wsddScopes.MatchBy);
        probes_ = probe(wsdd__Probe->wsdd__Types, &scopes);


        responce_.resize(probes_.size());
        wsdd__ProbeResponse->ProbeMatch.clear();
        wsdd__ProbeResponse->ProbeMatch.reserve(responce_.size());

        for (auto i = 0; i < probes_.size(); ++i) {
            wsa__EndpointReferenceType endpoint;
            endpoint.soap_default(this);
            responce_[i].wsa__EndpointReference = &endpoint;
            responce_[i].wsdd__Types = wsdd__Probe->wsdd__Types;
            responce_[i].wsdd__Scopes = wsdd__Probe->wsdd__Scopes;
            responce_[i].wsdd__XAddrs = &probes_[i];
            responce_[i].wsdd__MetadataVersion = 0;

            wsdd__ProbeResponse->ProbeMatch.push_back(&responce_[i]);
        }
   }


private:
    Wsa wsa_;
    std::vector<std::string> probes_;
    std::vector<wsdd__ProbeMatchType> responce_;
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
//    wsdd__HelloType req;
//    soap_default_wsdd__HelloType(impl_, &req);
//    req.wsa__EndpointReference.Address = const_cast<char *>(arg.endpoint.c_str());
//    req.Types = const_cast<char *>(arg.type.c_str());

//    wsdd__ScopesType req_scopes;
//    if (!scopes.empty()) {
//        soap_default_wsdd__ScopesType(impl_, &req_scopes);
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
//    wsdd__ByeType req;
//    soap_default_wsdd__ByeType(impl_, &req);
//#ifdef SOAP_WSA_2005
//    req.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#else
//    req.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#endif
//    req.Types = const_cast<char *>(types.c_str());

//    wsdd__ScopesType req_scopes;
//    if (!scopes.empty()) {
//        soap_default_wsdd__ScopesType(impl_, &req_scopes);
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
//    wsdd__ResolveType req;
//    soap_default_wsdd__ResolveType(impl_, &req);
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
//    wsdd__ResolveMatchType match;
//    soap_default_wsdd__ResolveMatchType(impl_, &match);
//#ifdef SOAP_WSA_2005
//    match.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#else
//    match.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
//#endif
//    match.Types = const_cast<char *>(types.c_str());

//    wsdd__ScopesType match_scopes;
//    if (!scopes.empty()) {
//      soap_default_wsdd__ScopesType(impl_, &match_scopes);
//      match_scopes.__item = const_cast<char *>(scopes.c_str());
//      match_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
//      match.Scopes = &match_scopes;
//    }
//    match.XAddrs = const_cast<char *>(xaddrs.c_str());
//    match.MetadataVersion = metadataVersion;


//    wsdd__ResolveMatchesType res;
//    soap_default_wsdd__ResolveMatchesType(impl_, &res);
//    res.ResolveMatch = &match;
//    if (impl_->send_ResolveMatches(endpoint.c_str(), ACTION.c_str(), &res) != 0)
//        throw SoapException(impl_);


//}


//Discovery::ProbeMatch::List_t Discovery::getProbeMatches()
//{
//    Discovery::ProbeMatch::List_t rv;

//    // managed mode: receive the matches
//    struct __wsdd__ProbeMatches res;
//    wsdd__ProbeMatchesType probe_matches;
//    soap_default_wsdd__ProbeMatchesType(impl_, &probe_matches);
//    res.wsdd__ProbeMatches = &probe_matches;

//    if (impl_->recv_ProbeMatches(res) != 0)
//        throw SoapException(impl_);

//    impl_->checkHeader("ProbeMatches wrong header");
//    if (res.wsdd__ProbeMatches == nullptr) {
//        if (soap_wsa_sender_fault(impl_, "WSDD ProbeMatches incomplete", nullptr) == 0)
//            throw SoapException(impl_);
//    }

//    for (int i = 0; i < res.wsdd__ProbeMatches->__sizeProbeMatch; ++i) {
//        auto & probe_match = res.wsdd__ProbeMatches->ProbeMatch[i];
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
//    struct __wsdd__ResolveMatches res;
//    soap_default_wsdd__ResolveMatchesType(impl_, res.wsdd__ResolveMatches);
//    if (impl_->recv_ResolveMatches(res) != 0)
//        throw SoapException(impl_);

//    impl_->checkHeader("WSDD ResolveMatches header incomplete");
//    if (res.wsdd__ResolveMatches == nullptr || res.wsdd__ResolveMatches->ResolveMatch == nullptr)
//        if (soap_wsa_sender_fault(impl_, "WSDD ResolveMatches incomplete", nullptr) == 0)
//            throw SoapException(impl_);

//    auto resolve_match = res.wsdd__ResolveMatches->ResolveMatch;
//    rv.endpoint.first = resolve_match->wsa__EndpointReference.Address;
//    rv.types = resolve_match->Types;
//    rv.scopes.item = resolve_match->Scopes->__item;
//    rv.scopes.matchBy = resolve_match->Scopes->MatchBy;
//    rv.xaddrs = resolve_match->XAddrs;
//    rv.version = resolve_match->MetadataVersion;

//    return rv;
//}


template<>
std::unique_ptr<DiscoveryLookup> service()
{
    return std::unique_ptr<DiscoveryLookup>(new DiscoveryLookupService);
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
