#include <onvifxx/remotediscovery.h>


int main(int argc, char ** argv)
{
    onvif::RemoteDiscovery * client = onvif::RemoteDiscovery::client();
    onvif::RemoteDiscovery * server = onvif::RemoteDiscovery::server();
    delete client;
    delete server;
}


