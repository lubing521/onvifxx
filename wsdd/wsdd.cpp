#include "wsdd.hpp"
#include "wsaapi.h"
#include "../sock/sock.hpp"
#include <onvifxx/onvifxx.hpp>
#include <soapwsddProxy.h>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include <mutex>
#include <thread>
#include <sstream>

namespace onvifxx {

const std::string TO_TS_URL = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";

class ProbeMatches : public wsdd__ProbeMatchesType
{
public:
    ProbeMatches(const Wsdd::ProbeMatch::List_t & probeMatches)
    {
        this->__sizeProbeMatch = probeMatches.size();
        this->ProbeMatch = new wsdd__ProbeMatchType[this->__sizeProbeMatch];

        for (std::size_t i = 0, I = probeMatches.size(); i < I; ++i) {
            this->ProbeMatch[i].wsa__EndpointReference.Address =
                    const_cast<char *>(probeMatches[i].endpoint.first.c_str());

            this->ProbeMatch[i].Types = const_cast<char *>(probeMatches[i].types.c_str());

            this->ProbeMatch[i].Scopes = new wsdd__ScopesType;
            this->ProbeMatch[i].Scopes->__item =
                    const_cast<char *>(probeMatches[i].scopes.item.c_str());
            this->ProbeMatch[i].Scopes->MatchBy =
                    const_cast<char *>(probeMatches[i].scopes.matchBy.c_str());

            this->ProbeMatch[i].XAddrs = const_cast<char *>(probeMatches[i].xaddrs.c_str());

            this->ProbeMatch[i].MetadataVersion = probeMatches[i].version;
        }
    }

    virtual ~ProbeMatches()
    {
        for (int i = 0; i < this->__sizeProbeMatch; ++i)
            delete this->ProbeMatch[i].Scopes;

        delete[] this->ProbeMatch;
    }
};

class Wsdd::Impl :
        public wsddProxy,
        private Sock
{
    typedef boost::asio::ip::tcp::iostream TcpIoStream_t;
    typedef boost::asio::basic_socket_iostream<boost::asio::ip::udp> UdpIoStream_t;

public:
    Impl() : Sock(this)
    {
        static const Namespace namespaces[] =
        {
            {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
            {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
            {"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
            {"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
            {"wsa", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL, NULL},
            {"wsdd", "http://schemas.xmlsoap.org/ws/2005/04/discovery", NULL, NULL},
            {"dn", "http://www.onvif.org/ver10/network/wsdl", NULL, NULL},
            {NULL, NULL, NULL, NULL}
        };
        soap_set_namespaces(this, namespaces);
    }

    static uint & instanceId()
    {
        static uint rv = 0;
        return rv;
    }

    static std::string & sequenceId()
    {
        static std::string rv;
        return rv;
    }

    static uint & messageNumber()
    {
        static uint rv = 1;
        return rv;
    }

    static std::string toString(const char * s)
    {
        return s == nullptr ? std::string() : std::string(s);
    }

    void setAppSequence()
    {
        if (!this->header->wsdd__AppSequence) {
            this->header->wsdd__AppSequence = new wsdd__AppSequenceType;
            soap_default_wsdd__AppSequenceType(this, this->header->wsdd__AppSequence);
        }

        static std::mutex mutex;

        std::lock_guard<std::mutex> lock(mutex);
        UNUSED(lock);
        this->header->wsdd__AppSequence->InstanceId = instanceId();
        this->header->wsdd__AppSequence->SequenceId = const_cast<char *>(sequenceId().c_str());
        this->header->wsdd__AppSequence->MessageNumber = messageNumber()++;
    }

    void resetAppSequence()
    {
        if (this->header) {
            delete this->header->wsdd__AppSequence;
            this->header->wsdd__AppSequence = nullptr;
        }
    }

    void delay()
    {
        const auto timeout = 1000 * (soap_random % APP_MAX_DELAY);
        ::usleep(timeout);
    }

    void checkHeader(const std::string & faultMessage)
    {
        // check WSA
        if (soap_wsa_check(this))
            throw SoapException(this);

        // check WSDD header
        if (soap_header() == nullptr) {
            if (soap_wsa_sender_fault(this, faultMessage.c_str(), nullptr) != 0)
                throw SoapException(this);
        }

        if (soap_header()->wsdd__AppSequence != nullptr) {
            wsdd__AppSequenceType * seq = soap_header()->wsdd__AppSequence;
            instanceId() = soap_header()->wsdd__AppSequence->InstanceId;
            sequenceId() = Impl::toString(soap_header()->wsdd__AppSequence->SequenceId);
            messageNumber() = soap_header()->wsdd__AppSequence->MessageNumber;
        }
    }
};

Wsdd::Wsdd() :
    impl_(new Impl)
{
}

Wsdd::~Wsdd()
{
    delete impl_;
    impl_ = nullptr;
}

void Wsdd::setInstanceId(uint instanceId)
{
    Impl::instanceId() = instanceId;
}

void Wsdd::setSequenceId(const std::string & sequenceId)
{
    Impl::sequenceId() = sequenceId;
}

std::string Wsdd::genUuid()
{
    return soap_wsa_rand_uuid(impl_) + 4;
}

void Wsdd::hello(Mode mode, const std::string & endpoint, const std::string & messageId,
                 const std::string & relatesTo, const std::string & endpointRef,
                 const std::string & types, const std::string & scopes, const std::string & matchBy,
                 const std::string & xaddrs, uint metadataVersion)
{
    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/Hello";

    // SOAP Header
    const std::string & dst = mode == MODE_ADHOC ? endpoint : TO_TS_URL;
    soap_wsa_request(impl_, messageId.c_str(), dst.c_str(), ACTION.c_str());
    soap_wsa_add_RelatesTo(impl_, relatesTo.c_str());

#ifdef SOAP_WSA_2005
    impl_->setAppSequence();
#else
    if (mode == MODE_ADHOC)
        impl_->setAppSequence();
    else
        impl_->resetAppSequence();
#endif

    // Hello
    wsdd__HelloType req;
    soap_default_wsdd__HelloType(impl_, &req);
#ifdef SOAP_WSA_2005
    req.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#else
    req.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#endif
    req.Types = const_cast<char *>(types.c_str());

    wsdd__ScopesType req_scopes;
    if (!scopes.empty()) {
        soap_default_wsdd__ScopesType(impl_, &req_scopes);
        req_scopes.__item = const_cast<char *>(scopes.c_str());
        req_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
        req.Scopes = &req_scopes;
    }
    req.XAddrs = const_cast<char *>(xaddrs.c_str());
    req.MetadataVersion = metadataVersion;

    if (impl_->send_Hello(endpoint.c_str(), ACTION.c_str(), &req) != 0)
        throw SoapException(impl_);
}

void Wsdd::bye(Mode mode, const std::string & endpoint, const std::string & messageId,
               const std::string & endpointRef, const std::string & types,
               const std::string & scopes, const std::string & matchBy, const std::string & xaddrs,
               uint metadataVersion)
{
    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/Bye";

    // SOAP Header
    const std::string & dst = mode == MODE_ADHOC ? endpoint : TO_TS_URL;
    soap_wsa_request(impl_, messageId.c_str(), dst.c_str(), ACTION.c_str());
#ifdef SOAP_WSA_2005
    impl_->setAppSequence();
#else
    if (mode == MODE_ADHOC)
        impl_->setAppSequence();
    else
        impl_->resetAppSequence();
#endif

    // Bye
    wsdd__ByeType req;
    soap_default_wsdd__ByeType(impl_, &req);
#ifdef SOAP_WSA_2005
    req.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#else
    req.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#endif
    req.Types = const_cast<char *>(types.c_str());

    wsdd__ScopesType req_scopes;
    if (!scopes.empty()) {
        soap_default_wsdd__ScopesType(impl_, &req_scopes);
        req_scopes.__item = const_cast<char *>(scopes.c_str());
        req_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
        req.Scopes = &req_scopes;
    }
    req.XAddrs = const_cast<char *>(xaddrs.c_str());
    req.MetadataVersion = &metadataVersion;

    if (impl_->send_Bye(endpoint.c_str(), ACTION.c_str(), &req) != 0)
        throw SoapException(impl_);
}

void Wsdd::probe(To to, const std::string & endpoint, const std::string & messageId,
                 const std::string & replyTo, const std::string & types, const std::string & scopes,
                 const std::string & matchBy)
{
    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/Probe";

    // SOAP Header
    const std::string & dst = to == TO_TS ? endpoint : TO_TS_URL;
    soap_wsa_request(impl_, messageId.c_str(), dst.c_str(), ACTION.c_str());
    soap_wsa_add_ReplyTo(impl_, replyTo.c_str());
    impl_->resetAppSequence();

    /* Probe */
    wsdd__ProbeType req;
    soap_default_wsdd__ProbeType(impl_, &req);
    req.Types = const_cast<char *>(types.c_str());

    wsdd__ScopesType req_scopes;
    if (!scopes.empty()) {
      soap_default_wsdd__ScopesType(impl_, &req_scopes);
      req_scopes.__item = const_cast<char *>(scopes.c_str());
      req_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
      req.Scopes = &req_scopes;
    }

    //impl_->connectTo(endpoint);
    if (impl_->send_Probe(endpoint.c_str(), ACTION.c_str(), &req) != 0)
        throw SoapException(impl_);

    //std::cerr << static_cast<std::basic_iostream<char> *>(impl_->os)->rdbuf() << std::endl;
}

void Wsdd::resolve(To to, const std::string & endpoint, const std::string & messageId,
                   const std::string & replyTo, const std::string & endpointRef)
{
    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/Resolve";

    // SOAP Header
    const std::string & dst = to == TO_TS ? endpoint : TO_TS_URL;
    soap_wsa_request(impl_, messageId.c_str(), dst.c_str(), ACTION.c_str());
    soap_wsa_add_ReplyTo(impl_, replyTo.c_str());
    impl_->resetAppSequence();

    /* Probe */
    wsdd__ResolveType req;
    soap_default_wsdd__ResolveType(impl_, &req);
#ifdef SOAP_WSA_2005
    req.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#else
    req.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#endif

    if (impl_->send_Resolve(endpoint.c_str(), ACTION.c_str(), &req) != 0)
        throw SoapException(impl_);
}

void Wsdd::probeMatches(const std::string & endpoint, const std::string & messageId,
                        const std::string & relatesTo, const std::string & to,
                        const ProbeMatch::List_t & matches)
{
    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/ProbeMatches";

    // 0..APP_MAX_DELAY ms delay
    impl_->delay();

    // SOAP Header
    soap_wsa_request(impl_, messageId.c_str(), to.c_str(), ACTION.c_str());
    soap_wsa_add_RelatesTo(impl_, relatesTo.c_str());
    impl_->setAppSequence();

    // ProbeMatches
    ProbeMatches probe_matches(matches);
    if (impl_->send_ProbeMatches(endpoint.c_str(), ACTION.c_str(), &probe_matches) != 0)
        throw SoapException(impl_);


}

void Wsdd::resolveMatches(const std::string & endpoint, const std::string & messageId,
                          const std::string & relatesTo, const std::string & to,
                          const std::string & endpointRef, const std::string & types,
                          const std::string & scopes, const std::string & matchBy,
                          const std::string & xaddrs, uint metadataVersion)
{
    static const std::string ACTION = SOAP_NAMESPACE_OF_wsdd"/ResolveMatches";

    // 0..APP_MAX_DELAY ms delay
    impl_->delay();

    // SOAP Header
    soap_wsa_request(impl_, messageId.c_str(), to.c_str(), ACTION.c_str());
    soap_wsa_add_RelatesTo(impl_, relatesTo.c_str());
    impl_->setAppSequence();

    // ResolveMatches
    wsdd__ResolveMatchType match;
    soap_default_wsdd__ResolveMatchType(impl_, &match);
#ifdef SOAP_WSA_2005
    match.wsa5__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#else
    match.wsa__EndpointReference.Address = const_cast<char *>(endpointRef.c_str());
#endif
    match.Types = const_cast<char *>(types.c_str());

    wsdd__ScopesType match_scopes;
    if (!scopes.empty()) {
      soap_default_wsdd__ScopesType(impl_, &match_scopes);
      match_scopes.__item = const_cast<char *>(scopes.c_str());
      match_scopes.MatchBy = const_cast<char *>(matchBy.c_str());
      match.Scopes = &match_scopes;
    }
    match.XAddrs = const_cast<char *>(xaddrs.c_str());
    match.MetadataVersion = metadataVersion;


    wsdd__ResolveMatchesType res;
    soap_default_wsdd__ResolveMatchesType(impl_, &res);
    res.ResolveMatch = &match;
    if (impl_->send_ResolveMatches(endpoint.c_str(), ACTION.c_str(), &res) != 0)
        throw SoapException(impl_);


}


Wsdd::ProbeMatch::List_t Wsdd::getProbeMatches()
{
    Wsdd::ProbeMatch::List_t rv;

    // managed mode: receive the matches
    struct __wsdd__ProbeMatches res;
    wsdd__ProbeMatchesType probe_matches;
    soap_default_wsdd__ProbeMatchesType(impl_, &probe_matches);
    res.wsdd__ProbeMatches = &probe_matches;

    if (impl_->recv_ProbeMatches(res) != 0)
        throw SoapException(impl_);

    impl_->checkHeader("ProbeMatches wrong header");
    if (res.wsdd__ProbeMatches == nullptr) {
        if (soap_wsa_sender_fault(impl_, "WSDD ProbeMatches incomplete", nullptr) == 0)
            throw SoapException(impl_);
    }

    for (int i = 0; i < res.wsdd__ProbeMatches->__sizeProbeMatch; ++i) {
        auto & probe_match = res.wsdd__ProbeMatches->ProbeMatch[i];
        rv.push_back(ProbeMatch());

        rv.back().endpoint.first    = Impl::toString(probe_match.wsa__EndpointReference.Address);
        rv.back().types             = Impl::toString(probe_match.Types);
        rv.back().scopes.item       = Impl::toString(probe_match.Scopes->__item);
        rv.back().scopes.matchBy    = Impl::toString(probe_match.Scopes->MatchBy);
        rv.back().xaddrs            = Impl::toString(probe_match.XAddrs);
        rv.back().version           = probe_match.MetadataVersion;
    }

    return rv;
}

Wsdd::ResolveMatch Wsdd::getResolveMatch()
{
    ResolveMatch rv;

    // managed mode: receive the matches
    struct __wsdd__ResolveMatches res;
    soap_default_wsdd__ResolveMatchesType(impl_, res.wsdd__ResolveMatches);
    if (impl_->recv_ResolveMatches(res) != 0)
        throw SoapException(impl_);

    impl_->checkHeader("WSDD ResolveMatches header incomplete");
    if (res.wsdd__ResolveMatches == nullptr || res.wsdd__ResolveMatches->ResolveMatch == nullptr)
        if (soap_wsa_sender_fault(impl_, "WSDD ResolveMatches incomplete", nullptr) == 0)
            throw SoapException(impl_);

    auto resolve_match = res.wsdd__ResolveMatches->ResolveMatch;
    rv.endpoint.first = resolve_match->wsa__EndpointReference.Address;
    rv.types = resolve_match->Types;
    rv.scopes.item = resolve_match->Scopes->__item;
    rv.scopes.matchBy = resolve_match->Scopes->MatchBy;
    rv.xaddrs = resolve_match->XAddrs;
    rv.version = resolve_match->MetadataVersion;

    return rv;
}

//int Wsdd::listen(int timeout)
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
}
