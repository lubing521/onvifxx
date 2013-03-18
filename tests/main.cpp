#include <onvif/remotediscovery.h>


int main(int argc, char ** argv)
{
    onvif::RemoteDiscovery * rd = onvif::RemoteDiscovery::create();
    delete rd;
}


