#include <onvifxx/media.hpp>
#include <onvifMediaBindingProxy.h>

namespace onvifxx {

struct MediaClient : MediaBindingProxy, Media
{
    virtual	Capabilities getServiceCapabilities()
    {
        int result = MediaBindingProxy::GetServiceCapabilities(0, 0);
        if (result != SOAP_OK)
            throw std::exception();

        return Capabilities();
    }
};

Media * createMediaClient()
{
    return new MediaClient;
}

} // namespace onvifxx
