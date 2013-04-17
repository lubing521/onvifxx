#include <onvifxx/discovery.hpp>
#include "../wsdd/wsdd.hpp"

#include <boost/array.hpp>

#include <sstream>

namespace onvifxx {
namespace client {

struct Discovery::Impl : Wsdd
{
};

Discovery::Discovery() :
    impl_(new Impl)
{
}

Discovery::~Discovery()
{
    delete impl_;
    impl_ = 0;
}


DiscoveryLookup::ProbeMatch::List_t Discovery::probe(Probe probe)
{
    DiscoveryLookup::ProbeMatch::List_t rv;

    const auto id = impl_->genUuid();
    impl_->probe(Wsdd::TO_TS, MULTICAST_ENDPOINT,
                 id, "", probe.type, probe.scope.item, probe.scope.matchBy);

    while (true) try {
        for (const auto & probeMatch : impl_->getProbeMatches()) {
            rv.push_back(DiscoveryLookup::ProbeMatch());
            rv.back().endpoint = probeMatch.endpoint;
            rv.back().probe.type = probeMatch.types;
            rv.back().probe.scope.item = probeMatch.scopes.item;
            rv.back().probe.scope.matchBy = probeMatch.scopes.matchBy;
            rv.back().xaddr = probeMatch.xaddrs;
            rv.back().version = probeMatch.version;
        }
    } catch (SoapException & ex) {
        if (ex.code() == 0)
            break;
        else
            throw;
    }

    return rv;
}


RemoteDiscovery::ResolveType Discovery::hello(RemoteDiscovery::HelloType arg)
{
    return RemoteDiscovery::ResolveType();
}

RemoteDiscovery::ResolveType Discovery::bye(RemoteDiscovery::ByeType arg)
{
    return RemoteDiscovery::ResolveType();
}



} // namespace client
} // namespace onvifxx
