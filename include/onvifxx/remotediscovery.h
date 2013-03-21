#ifndef ONVIF_REMOTEDICOVERY_H
#define ONVIF_REMOTEDICOVERY_H

#include <memory>

namespace onvifxx {

template<class T>
T * createProxy();

template<class T>
T * createService();

struct HelloType { };
struct ByeType { };
struct ResolveType { };

struct RemoteDiscovery
{
    virtual ResolveType hello(HelloType hello) = 0;
    virtual ResolveType bye(ByeType bye) = 0;
};


} // namespace onvifxx

#endif // ONVIF_REMOTEDICOVERY_H
