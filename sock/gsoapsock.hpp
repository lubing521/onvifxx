#ifndef ONVIFXX_GSOAPSOCK_HPP
#define ONVIFXX_GSOAPSOCK_HPP

#include <stdsoap2.h>

namespace onvifxx {

class GsoapSock
{
public:
    static void assign(struct soap * soap);

private:
    // Called from a client proxy to open a connection to a Web Service located at endpoint. Input parameters host and port are micro-parsed from endpoint. Should return a valid file descriptor, or SOAP_INVALID_SOCKET and soap->error set to an error code. Built-in gSOAP function: tcp_connect
    static int open(struct soap * soap, const char * endpoint, const char * host, int port);

    // Called by client proxy multiple times, to close a socket connection before a new socket connection is established and at the end of communications when the SOAP_IO_KEEPALIVE flag is not set and soap.keep_alive ≠ 0 (indicating that the other party supports keep alive). Should return SOAP_OK, or a gSOAP error code. Built-in gSOAP function: tcp_disconnect
    static int close(struct soap * soap);

    // Called for all send operations to emit contents of s of length n. Should return SOAP_OK, or a gSOAP error code. Built-in gSOAP function: fsend
    static int send(struct soap * soap, const char * s, size_t n);

    // Called for all receive operations to fill buffer s of maximum length n. Should return the number of bytes read or 0 in case of an error, e.g. EOF. Built-in gSOAP function: frecv
    static size_t recv(struct soap * soap, char * s, size_t n);

    // Called when an unknown XML element was encountered on the input. The tag parameter is the offending XML element tag name. Should return SOAP_OK, or a gSOAP error code such as SOAP_TAG_MISMATCH to throw an exception. Built-in gSOAP function: none.
    static int ignore(struct soap * soap, const char * tag);

    // When non-NULL, this callback is called for all client-to-server connect operations instead of the built-in socket connect code. Therefore, it can be used to override the built-in connection establishment. Parameter endpoint contains the server endpoint URL, host the domain name or IP, and port the port number. Should return SOAP_OK, or a gSOAP error code. Built-in gSOAP function: none
    static int connect(struct soap * soap, const char * endpoint, const char * host, int port);

    // Called by soap_accept. This is a wrapper routine for accept. Given master socket s should return a valid socket descriptor or SOAP_INVALID_SOCKET and set soap->error to an error code. Built-in gSOAP function: tcp_accept
    static int accept(struct soap * soap, int s, struct sockaddr *a, int * n);

    // Called by soap_bind if a host name is given and soap_connect to resolve a domain name addr. Should set in_addr *a and return SOAP_OK or return SOAP_ERR upon failure.
    // Built-in gSOAP function: tcp_gethost
    static int resolve(struct soap * soap, const char * addr, struct in_addr * inaddr);

    // Used by clients to check if the server is still responsive.
    // Built-in gSOAP function: soap_poll
    static int poll(struct soap * soap);

    // Called after successful invocation of a server operation in the server loop, immediately after sending the response to a client. Can be used to clean up resources (e.g. using soap_end()) while serving a long sequence of keep-alive connections. Should return SOAP_OK, or set soap->error to a gSOAP error code and return soap->error. Built-in gSOAP function: none.
    //static int serveloop(struct soap * soap);
};

} // namespace onvifxx

#endif // ONVIFXX_GSOAPSOCK_HPP
