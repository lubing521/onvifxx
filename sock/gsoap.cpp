#include <boost/asio.hpp>
#include <stdsoap2.h>

//Called from a client proxy to open a connection to a Web Service located at endpoint. Input parameters host and port are micro-parsed from endpoint. Should return a valid file descriptor, or SOAP_INVALID_SOCKET and soap->error set to an error code. Built-in gSOAP function: tcp_connect
/*SOAP_SOCKET soap_fopen(struct soap *soap, const char *endpoint, const char *host, int port)
{

}*/

//Called by client proxy multiple times, to close a socket connection before a new socket connection is established and at the end of communications when the SOAP_IO_KEEPALIVE flag is not set and soap.keep_alive ≠ 0 (indicating that the other party supports keep alive). Should return SOAP_OK, or a gSOAP error code. Built-in gSOAP function: tcp_disconnect
//int soap.fclose)(struct soap *soap)


int soap_accept(struct soap * soap)
{
    return 0;
}
