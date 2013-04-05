#include "gsoapsock.hpp"

#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/array.hpp>

#include <iostream>

namespace onvifxx {

void GsoapSock::assign(struct soap * soap)
{
    //boost::asio::io_service ios;

    assert(soap->fpost != nullptr);
    assert(soap->fget != nullptr);
    assert(soap->fput != nullptr);
    assert(soap->fdel != nullptr);
    assert(soap->fopt != nullptr);

    assert(soap->fhead == nullptr);

    assert(soap->fform != nullptr);
    assert(soap->fposthdr != nullptr);
    assert(soap->fresponse != nullptr);
    assert(soap->fparse != nullptr);

    assert(soap->fparsehdr == nullptr);
    assert(soap->fheader == nullptr);
    assert(soap->fresolve == nullptr);
    assert(soap->fconnect == nullptr);
    assert(soap->fdisconnect == nullptr);
    assert(soap->fclosesocket == nullptr);
    assert(soap->fshutdownsocket == nullptr);
    assert(soap->fopen == nullptr);
    assert(soap->faccept == nullptr);
    assert(soap->fclose == nullptr);
    assert(soap->fsend == nullptr);
    assert(soap->frecv == nullptr);
    assert(soap->fpoll == nullptr);
    assert(soap->fseterror == nullptr);
    assert(soap->fignore == nullptr);

    assert(soap->fserveloop != nullptr);

    assert(soap->fplugin == nullptr);
    assert(soap->fmalloc == nullptr);
    assert(soap->feltbegin == nullptr);
    assert(soap->feltendin == nullptr);
    assert(soap->feltbegout == nullptr);
    assert(soap->feltendout == nullptr);
    assert(soap->fprepareinitsend == nullptr);
    assert(soap->fprepareinitrecv == nullptr);
    assert(soap->fpreparesend == nullptr);
    assert(soap->fpreparerecv == nullptr);
    assert(soap->fpreparefinalsend == nullptr);
    assert(soap->fpreparefinalrecv == nullptr);
    assert(soap->ffiltersend == nullptr);
    assert(soap->ffilterrecv == nullptr);
    assert(soap->fdimereadopen == nullptr);
    assert(soap->fdimewriteopen == nullptr);
    assert(soap->fdimereadclose == nullptr);
    assert(soap->fdimewriteclose == nullptr);
    assert(soap->fdimeread == nullptr);
    assert(soap->fdimewrite == nullptr);
    assert(soap->fmimereadopen == nullptr);
    assert(soap->fmimewriteopen == nullptr);
    assert(soap->fmimereadclose == nullptr);
    assert(soap->fmimewriteclose == nullptr);
    assert(soap->fmimeread == nullptr);

    assert(soap->fmimewrite != nullptr);


    soap->fopen = GsoapSock::open;
    soap->fsend = GsoapSock::send;
    soap->frecv = GsoapSock::recv;
    soap->fignore = GsoapSock::ignore;
    soap->fconnect = GsoapSock::connect;
    soap->faccept = GsoapSock::accept;
    soap->fresolve = GsoapSock::resolve;
    soap->fpoll = GsoapSock::poll;
    //soap->fserveloop = GsoapSock::serveloop;
}

int GsoapSock::open(struct soap * soap, const char * endpoint, const char * host, int port)
{
    return SOAP_OK;
}

int GsoapSock::close(struct soap * soap)
{
    return SOAP_OK;
}

int GsoapSock::send(soap * soap, const char * s, size_t n)
{
    return SOAP_OK;
}

size_t GsoapSock::recv(soap * soap, char * s, size_t n)
{
    return SOAP_OK;
}

int GsoapSock::ignore(soap * soap, const char * tag)
{
    return SOAP_OK;
}

int GsoapSock::connect(soap * soap, const char * endpoint, const char * host, int port)
{
    return SOAP_OK;
}

int GsoapSock::accept(soap * soap, int s, sockaddr * a, int * n)
{
    return SOAP_OK;
}

int GsoapSock::resolve(soap * soap, const char * addr, in_addr * inaddr)
{
    static const std::string UDP_SCHEEMA   = "soap.udp://";
    static const std::string HTTP_SCHEEMA  = "http://";

    std::string url = addr;
    assert(!url.empty());

    std::string host = "";
    for (int i = 0; i < 2; ++i) {
        const auto & scheema = (i == 0 ? HTTP_SCHEEMA : UDP_SCHEEMA);
        if (url.substr(0, scheema.size()) == scheema)
            host    = url.substr(scheema.size());
    }

    assert(!host.empty());
    std::size_t port_pos = host.find(':');
    if (port_pos != std::string::npos)
        host = host.substr(0, port_pos);

    assert(!host.empty());
    boost::asio::io_service ios;
    boost::asio::ip::tcp::resolver r(ios);
    boost::asio::ip::tcp::resolver::query q(host, "");
    boost::asio::ip::tcp::endpoint ep = *r.resolve(q);
    inaddr->s_addr = ep.address().to_v4().to_ulong();

    return SOAP_OK;


}

int GsoapSock::poll(soap * soap)
{
    return SOAP_OK;
}

//int GsoapSock::serveloop(soap * soap)
//{
//    return SOAP_OK;
//}


} // namespace onvifxx

int soap_accept(struct soap * soap)
{
    return 0;
}
