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

typedef onvifxx::Proxy<onvifxx::RemoteDiscovery> Proxy_t;
typedef onvifxx::Service<onvifxx::RemoteDiscovery> Service_t;

struct RemoteDiscoveryService : onvifxx::RemoteDiscovery
{
    boost::mutex readyMutex;

    virtual void hello(std::string * types, Scopes_t * scopes)
    {
        std::clog << "hello("
                  << (types != nullptr ? *types : "") << ", "
                  << (scopes != nullptr ? scopes->first : "")
                  << ")" << std::endl;
    }

    virtual void bye(std::string * types, Scopes_t * scopes)
    {
        std::clog << "bye("
                  << (types != nullptr ? *types : "") << ", "
                  << (scopes != nullptr ? scopes->first : "")
                  << ")" << std::endl;
    }

    virtual ProbeMatches_t probe(std::string * types, Scopes_t * scopes)
    {
        std::clog << "probe("
                  << (types != nullptr ? *types : "") << ", "
                  << (scopes != nullptr ? scopes->first : "")
                  << ")" << std::endl;
        return ProbeMatches_t();
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

        std::string types = "dn:NetworkVideoTransmitter";
        boost::scoped_ptr<Proxy_t>  proxy(onvifxx::RemoteDiscovery::proxy());
        onvifxx::RemoteDiscovery::ProbeMatches_t matches = proxy->probe(&types, nullptr);
        std::copy(matches.begin(), matches.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

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
