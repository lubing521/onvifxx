#include <onvifxx/discovery.hpp>
#include "../wsdd/wsdd.hpp"

#include <boost/array.hpp>

namespace onvifxx {
namespace client {

struct Discovery::Impl : Wsdd
{
    DiscoveryLookup::ProbeMatch::List_t probeMatches;
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


void Discovery::probe(Probe probe)
{
    const auto id = impl_->genUuid();
    impl_->probe(Wsdd::TO_TS, "soap.udp://239.255.255.250:3702",
                 id, "", "dn:NetworkVideoTransmitter", "", "");

    const auto probe_matches1 = impl_->getProbeMatches();
    impl_->probeMatches.resize(probe_matches1.size());
    const auto probe_matches2 = impl_->getProbeMatches();
    impl_->probeMatches.resize(probe_matches2.size());
}

void Discovery::getProbeMatches(ProbeMatch::List_t & probeMatches)
{
    probeMatches.assign(impl_->probeMatches.begin(), impl_->probeMatches.end());
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
