#include "sock.hpp"
#include "tcp.hpp"
#include "udp.hpp"

#include <stdsoap2.h>

namespace onvifxx {

struct Endpoint
{
    std::string scheema;
    std::string host;
    std::string service;
    bool isUdp;

    static Endpoint fromUrl(const std::string & url)
    {
        BOOST_ASSERT(!url.empty());

        static const std::string HTTP_SCHEEMA  = "http://";
        static const std::string HTTPS_SCHEEMA   = "https://";
        static const std::string UDP_SCHEEMA   = "soap.udp://";


        Endpoint rv;
        rv.isUdp = false;

        for (int i = 0; i < 3; ++i) {
            switch (i) {
            case 0:
                rv.scheema = HTTP_SCHEEMA;
                break;
            case 1:
                rv.scheema = HTTPS_SCHEEMA;
                break;
            default:
                rv.scheema = UDP_SCHEEMA;
            }

            if (url.substr(0, rv.scheema.size()) == rv.scheema) {
                rv.host = url.substr(rv.scheema.size());
                rv.isUdp = i == 2;
                break;
            }
        }
        BOOST_ASSERT(!rv.host.empty());

        rv.service = "80";
        if (rv.scheema == HTTPS_SCHEEMA)
            rv.service = "443";

        const auto port_pos = rv.host.find(':');
        if (port_pos != std::string::npos) {
            rv.service = rv.host.substr(port_pos + 1);
            rv.host = rv.host.substr(0, port_pos);
        }

        BOOST_ASSERT(!rv.scheema.empty());
        BOOST_ASSERT(!rv.host.empty());
        BOOST_ASSERT(!rv.service.empty());
        return rv;
    }

    ulong inAddr() const
    {
        boost::asio::io_service ios;
        boost::asio::ip::tcp::resolver r(ios);
        boost::asio::ip::tcp::resolver::query q(host, service);
        boost::asio::ip::tcp::endpoint ep = *r.resolve(q);
        return ep.address().to_v4().to_ulong();
    }
};

//void connectTo(const std::string & url)
//{
//    TcpIoStream_t tcpStream;
//    UdpIoStream_t udpStream;

//    static const std::string HTTP_SCHEEMA  = "http://";
//    static const std::string UDP_SCHEEMA   = "soap.udp://";

//    bool isUdp = false;

//    BOOST_ASSERT(!url.empty());
//    std::string host = "";
//    for (int i = 0; i < 2; ++i) {
//        const auto & scheema = (i == 0 ? HTTP_SCHEEMA : UDP_SCHEEMA);
//        if (url.substr(0, scheema.size()) == scheema) {
//            host = url.substr(scheema.size());
//            isUdp = i == 1;
//        }
//    }


//    BOOST_ASSERT(!host.empty());
//    std::string service = "80";
//    std::size_t port_pos = host.find(':');
//    if (port_pos != std::string::npos) {
//        service = host.substr(port_pos + 1);
//        host = host.substr(0, port_pos);
//    }

//    BOOST_ASSERT(!host.empty());
//    BOOST_ASSERT(!service.empty());

//    if (isUdp) {
//        udpStream.connect(host, service);
//        is = &udpStream;
//        os = &udpStream;
//    } else {
//        tcpStream.connect(host, service);
//        is = &tcpStream;
//        os = &tcpStream;
//    }
//}

struct Sock::Impl
{
    soap * s;
    Ip * ip;

    struct
    {
        SOAP_SOCKET (*fopen)(struct soap*, const char*, const char*, int);
        int (*fclose)(struct soap*);
        int (*fsend)(struct soap*, const char*, size_t);
        size_t (*frecv)(struct soap*, char*, size_t);
        int (*fignore)(struct soap*, const char*);
        int (*fconnect)(struct soap*, const char*, const char*, int);
        SOAP_SOCKET (*faccept)(struct soap*, SOAP_SOCKET, struct sockaddr*, int *n);
        int (*fresolve)(struct soap*, const char*, struct in_addr* inaddr);
        int (*fpoll)(struct soap*);
        int (*fserveloop)(struct soap*);

    } context;



    static int open(soap * s, const char * endpoint, const char * host, int port)
    {
        return connect(s, endpoint, host, port);
    }

    static int close(soap * s)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        const auto result = impl->ip->close();
        if (result) {
            soap_set_sender_error(s, "failed to close", result.message().c_str(),
                                  impl->ip->isUdp() ? SOAP_UDP_ERROR : SOAP_TCP_ERROR);
            return SOAP_INVALID_SOCKET;
        }

        return SOAP_OK;
    }

    static int send(soap * s, const char * data, size_t n)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        const auto result = impl->ip->send(data, n);
        if (result) {
            soap_set_sender_error(s, "failed to send", result.message().c_str(),
                                  impl->ip->isUdp() ? SOAP_UDP_ERROR : SOAP_TCP_ERROR);
            return SOAP_INVALID_SOCKET;
        }

        return SOAP_OK;
    }

    static size_t recv(soap * s, char * data, size_t n)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        const auto result = impl->ip->recv(data, n);
        if (result) {
            soap_set_sender_error(s, "failed to recv", result.message().c_str(),
                                  impl->ip->isUdp() ? SOAP_UDP_ERROR : SOAP_TCP_ERROR);
            return SOAP_INVALID_SOCKET;
        }

        return SOAP_OK;
    }

    static int ignore(soap * s, const char * tag)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        return SOAP_OK;
    }

    static int connect(soap * s, const char * endpoint, const char * host, int port)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        std::unique_ptr<Ip> ip;
        if (Endpoint::fromUrl(endpoint).isUdp)
            ip.reset(new Udp);
        else
            ip.reset(new Tcp);

        const auto result = ip->connect(host, port);
        if (result) {
            soap_set_sender_error(s, "failed to connect", result.message().c_str(),
                                  ip->isUdp() ? SOAP_UDP_ERROR : SOAP_TCP_ERROR);
            return SOAP_INVALID_SOCKET;
        }

        delete impl->ip;
        impl->ip = ip.release();

        return SOAP_OK;
    }

    static int accept(soap * s, int h, sockaddr * a, int * n)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        return SOAP_OK;
    }

    static int resolve(soap * s, const char * addr, in_addr * inaddr)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        const auto endpoint = Endpoint::fromUrl(addr);
        inaddr->s_addr = endpoint.inAddr();
        return SOAP_OK;
    }

    static int poll(soap * s)
    {
        Impl * impl = static_cast<Impl *>(s->user);
        BOOST_ASSERT(impl->s == s);

        return SOAP_OK;
    }

    static int serveloop(soap * s)
    {
        return SOAP_OK;
    }
};

Sock::Sock(soap * s) :
    impl_(new Impl)
{
    BOOST_ASSERT(s != nullptr);
    BOOST_ASSERT(s->user == nullptr);

    BOOST_ASSERT(s->fpost != nullptr);
    BOOST_ASSERT(s->fget != nullptr);
    BOOST_ASSERT(s->fput != nullptr);
    BOOST_ASSERT(s->fdel != nullptr);
    BOOST_ASSERT(s->fopt != nullptr);
    BOOST_ASSERT(s->fhead != nullptr);

    BOOST_ASSERT(s->fform == nullptr);

    BOOST_ASSERT(s->fposthdr != nullptr);
    BOOST_ASSERT(s->fresponse != nullptr);
    BOOST_ASSERT(s->fparse != nullptr);
    BOOST_ASSERT(s->fparsehdr != nullptr);

    BOOST_ASSERT(s->fheader == nullptr);
    BOOST_ASSERT(s->fresolve == nullptr);
    BOOST_ASSERT(s->fconnect == nullptr);
    BOOST_ASSERT(s->fdisconnect == nullptr);

    BOOST_ASSERT(s->fclosesocket != nullptr);
    BOOST_ASSERT(s->fshutdownsocket != nullptr);
    BOOST_ASSERT(s->fopen != nullptr);
    BOOST_ASSERT(s->faccept != nullptr);
    BOOST_ASSERT(s->fclose != nullptr);
    BOOST_ASSERT(s->fsend != nullptr);
    BOOST_ASSERT(s->frecv != nullptr);
    BOOST_ASSERT(s->fpoll != nullptr);

    BOOST_ASSERT(s->fseterror == nullptr);
    BOOST_ASSERT(s->fignore == nullptr);
    BOOST_ASSERT(s->fserveloop == nullptr);

    BOOST_ASSERT(s->fplugin != nullptr);

    BOOST_ASSERT(s->fmalloc == nullptr);
    BOOST_ASSERT(s->feltbegin == nullptr);
    BOOST_ASSERT(s->feltendin == nullptr);
    BOOST_ASSERT(s->feltbegout == nullptr);
    BOOST_ASSERT(s->feltendout == nullptr);
    BOOST_ASSERT(s->fprepareinitsend == nullptr);
    BOOST_ASSERT(s->fprepareinitrecv == nullptr);
    BOOST_ASSERT(s->fpreparesend == nullptr);
    BOOST_ASSERT(s->fpreparerecv == nullptr);
    BOOST_ASSERT(s->fpreparefinalsend == nullptr);
    BOOST_ASSERT(s->fpreparefinalrecv == nullptr);
    BOOST_ASSERT(s->ffiltersend == nullptr);
    BOOST_ASSERT(s->ffilterrecv == nullptr);
    BOOST_ASSERT(s->fdimereadopen == nullptr);
    BOOST_ASSERT(s->fdimewriteopen == nullptr);
    BOOST_ASSERT(s->fdimereadclose == nullptr);
    BOOST_ASSERT(s->fdimewriteclose == nullptr);
    BOOST_ASSERT(s->fdimeread == nullptr);
    BOOST_ASSERT(s->fdimewrite == nullptr);
    BOOST_ASSERT(s->fmimereadopen == nullptr);
    BOOST_ASSERT(s->fmimewriteopen == nullptr);
    BOOST_ASSERT(s->fmimereadclose == nullptr);
    BOOST_ASSERT(s->fmimewriteclose == nullptr);
    BOOST_ASSERT(s->fmimeread == nullptr);
    BOOST_ASSERT(s->fmimewrite == nullptr);


    impl_->context.fopen        = s->fopen;
    impl_->context.fclose       = s->fclose;
    impl_->context.fsend        = s->fsend;
    impl_->context.frecv        = s->frecv;
    impl_->context.fignore      = s->fignore;
    impl_->context.fconnect     = s->fconnect;
    impl_->context.faccept      = s->faccept;
    impl_->context.fresolve     = s->fresolve;
    impl_->context.fpoll        = s->fpoll;
    impl_->context.fserveloop   = s->fserveloop;

    s->fopen        = Impl::open;
    s->fclose       = Impl::close;
    s->fsend        = Impl::send;
    s->frecv        = Impl::recv;
    s->fignore      = Impl::ignore;
    s->fconnect     = Impl::connect;
    s->faccept      = Impl::accept;
    s->fresolve     = Impl::resolve;
    s->fpoll        = Impl::poll;
    s->fserveloop   = Impl::serveloop;

    s->user = impl_;
    impl_->s = s;
    impl_->ip = nullptr;
}

Sock::~Sock()
{
    impl_->s->fopen        = impl_->context.fopen;
    impl_->s->fclose       = impl_->context.fclose;
    impl_->s->fsend        = impl_->context.fsend;
    impl_->s->frecv        = impl_->context.frecv;
    impl_->s->fignore      = impl_->context.fignore;
    impl_->s->fconnect     = impl_->context.fconnect;
    impl_->s->faccept      = impl_->context.faccept;
    impl_->s->fresolve     = impl_->context.fresolve;
    impl_->s->fpoll        = impl_->context.fpoll;
    impl_->s->fserveloop   = impl_->context.fserveloop;

    impl_->s->user = nullptr;
    delete impl_;
    impl_ = nullptr;
}

} // namespace onvifxx

