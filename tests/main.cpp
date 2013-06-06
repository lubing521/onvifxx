#include <onvifxx/remotediscovery.hpp>
//#include <onvifxx/media.hpp>

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace asio = boost::asio;
typedef asio::ip::udp AsioUdp_t;


//struct DiscoveryService : onvifxx::Discovery
//{
//    virtual ResolveType hello(HelloType arg)
//    {
//        return onvifxx::DiscoveryServer::hello(arg);
//    }

//    virtual ResolveType bye(ByeType arg)
//    {
//        return onvifxx::DiscoveryServer::bye(arg);
//    }

//    virtual ProbeMatchesType probe(ProbeType arg)
//    {
//        return onvifxx::DiscoveryServer::probe(arg);
//    }

//};

typedef onvifxx::RemoteDiscovery RD_t;

typedef onvifxx::Proxy<RD_t> Proxy_t;
typedef onvifxx::Service<RD_t> Service_t;

struct RemoteDiscoveryService : RD_t
{
    boost::mutex readyMutex;

    virtual void hello(const Hello_t & arg)
    {
        std::clog << "hello("
                  << (arg.xaddrs != nullptr ? *arg.xaddrs : "") << ", "
                  << (arg.types != nullptr ? *arg.types : "") << ", "
                  << (arg.scopes != nullptr ? arg.scopes->item : "")
                  << ")" << std::endl;
    }

    virtual void bye(const Bye_t & arg)
    {
        std::clog << "bye("
                  << (arg.xaddrs != nullptr ? *arg.xaddrs : "") << ", "
                  << (arg.types != nullptr ? *arg.types : "") << ", "
                  << (arg.scopes != nullptr ? arg.scopes->item : "")
                  << ")" << std::endl;
    }

    virtual void probe(const Probe_t & arg)
    {
        std::clog << "probe("
                  << (arg.types != nullptr ? *arg.types : "") << ", "
                  << (arg.scopes != nullptr ? arg.scopes->item : "")
                  << ")" << std::endl;
    }

    void probeMatches(const ProbeMatches_t & arg, const std::string & relatesTo)
    {
        std::clog << "probeMatches(" <<  arg.size() << ", " << relatesTo << ")" << std::endl;
    }

    virtual void run()
    {
        try {
            boost::scoped_ptr<Service_t> service(onvifxx::RemoteDiscovery::service());
            BOOST_ASSERT(service->bind(this) != -1);

            readyMutex.unlock();
            for (int i = 0; i < 100000; ++i) {
                std::clog << i << std::endl;

                int socket = service->accept();
                BOOST_ASSERT(socket != -1);

                int error = service->serve();
                if (error == -1)
                    continue;
                if (error != 0)
                    throw onvifxx::SoapException(*service);

                service->destroy();
            }
        } catch(std::exception & ex) {
            std::clog << "FAIL: " << ex.what() << std::endl;
        }
    }
};

int main(int argc, char ** argv)
{
    UNUSED(argc);
    UNUSED(argv);

    try {
        RemoteDiscoveryService service;
        service.readyMutex.lock();
        boost::thread service_thread(boost::bind(&RemoteDiscoveryService::run, &service));

        boost::mutex::scoped_lock lock(service.readyMutex);
        UNUSED(lock);

        std::string address = "urn:uuid:05f1b46c-f29a-46f7-9140-e4bc00c8cea6";
        std::string xaddrs = "http://127.0.0.1/onvif/services";
        std::string types = "dn:NetworkVideoTransmitter";
        boost::scoped_ptr<Proxy_t>  proxy(RD_t::proxy());

        RD_t::EndpointReference_t endpoint = RD_t::EndpointReference_t();
        endpoint.address = &address;

        RD_t::Scopes_t scopes = RD_t::Scopes_t();
        scopes.item = "onvif://www.onvif.org/type/Test onvif://www.onvif.org/name/OnvifxxTest";

        RD_t::Hello_t hello = RD_t::Hello_t();
        hello.endpoint = &endpoint;
        hello.types = &types;
        hello.scopes = &scopes;
        hello.xaddrs = &xaddrs;
        hello.version = 1;
        proxy->hello(hello);

        RD_t::Bye_t bye = RD_t::Bye_t();
        bye.endpoint = &endpoint;
        bye.types = &types;
        bye.scopes = &scopes;
        bye.xaddrs = &xaddrs;
        bye.version = 1;
        proxy->bye(bye);

        RD_t::Probe_t probe = RD_t::Probe_t();
        probe.types = &types;
        proxy->probe(probe);

        RD_t::ProbeMatch_t probeMatch = RD_t::ProbeMatch_t();
        probeMatch.endpoint = &endpoint;
        probeMatch.types = &types;
        probeMatch.scopes = &scopes;
        probeMatch.xaddrs = &xaddrs;
        probeMatch.version = 1;
        RD_t::ProbeMatches_t probeMatches = RD_t::ProbeMatches_t();
        probeMatches.push_back(probeMatch);
        proxy->probeMatches(probeMatches, "");

        service_thread.join();

    } catch (const onvifxx::UnixException & ex) {
        std::cerr << "Error (" << ex.code() << ") " << ex.what() << "!" << std::endl;
        return 1;
    } catch (const std::exception & ex) {
        std::cerr << "Error (" << ex.what() << ")!" << std::endl;
        return 2;
    }

    return 0;
}
