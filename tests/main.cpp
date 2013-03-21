#include <onvifxx/remotediscovery.h>

int main(int argc, char ** argv)
{
//    onvif::RemoteDiscovery * client = onvif::RemoteDiscovery::client();
//    onvif::RemoteDiscovery * server = onvif::RemoteDiscovery::server();
//    delete client;
//    delete server;

    auto proxy = onvifxx::createProxy<onvifxx::RemoteDiscovery>();
    delete proxy;

    auto service = onvifxx::createService<onvifxx::RemoteDiscovery>();
    delete service;
}


