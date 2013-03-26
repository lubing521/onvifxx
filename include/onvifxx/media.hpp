#ifndef ONVIFXX_MEDIA
#define ONVIFXX_MEDIA

#include "onvifxx.hpp"

namespace onvifxx {

struct Media
{
    struct Capabilities {};

    virtual ~Media() { }

    virtual	Capabilities getServiceCapabilities() = 0;
};

Media * createMediaClient();

} // namespace onvifxx

#endif //ONVIFXX_MEDIA
