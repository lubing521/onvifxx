#include <onvifxx/remotediscovery.h>

int main(int argc, char ** argv)
{
    auto proxy = onvifxx::createProxy<onvifxx::RemoteDiscovery>();
    delete proxy;

    auto service = onvifxx::createService<onvifxx::RemoteDiscovery>();
    delete service;
