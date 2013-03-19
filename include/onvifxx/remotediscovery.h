#ifndef ONVIF_REMOTEDICOVERY_H
#define ONVIF_REMOTEDICOVERY_H

namespace onvif {

class RemoteDiscovery
{
public:
    struct HelloType { };
    struct ByeType { };
    struct ResolveType { };

    virtual ResolveType Hello(HelloType hello);
    virtual ResolveType Bye(ByeType  bye);

    static RemoteDiscovery * client();
    static RemoteDiscovery * server();

protected:
    RemoteDiscovery();
};

} // namespace onvif

#endif // ONVIF_REMOTEDICOVERY_H
