#include "wsdd.hpp"
#include "log.hpp"

#include <boost/thread.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

#include <iostream>

Wsdd::Wsdd() :
    signals_(ios_),
    stopped_(false)
{
    std::clog << Log::INFO << "start" << std::endl;
    signals_.add(SIGHUP);
    signals_.add(SIGINT);
    signals_.add(SIGQUIT);
    signals_.add(SIGTERM);
    signals_.add(SIGUSR1);
    signals_.async_wait(boost::bind(&Wsdd::signalHandler, this, _1, _2));

    scopes_.push_back("onvif://www.onvif.org/type/NetworkVideoTransmitter");
    scopes_.push_back("onvif://www.onvif.org/type/video_encoder");
    scopes_.push_back("onvif://www.onvif.org/type/audio_encoder");
    scopes_.push_back("onvif://www.onvif.org/type/ptz");
    scopes_.push_back("onvif://www.onvif.org/name/OnvifxxExample");
    scopes_.push_back("onvif://www.onvif.org/location/Anywhere");
    scopes_.push_back("onvif://www.onvif.org/hardware/OnvifxxEngine");
}

Wsdd::~Wsdd()
{
    std::clog << Log::INFO << "exit" << std::endl;
}

int Wsdd::exec(bool daemonize)
{
    if (daemonize && daemon(0, 0) < 0) {
        std::cerr << "Could not daemonize " << errno << std::endl;
        return 1;
    }

    work_.reset(new boost::asio::io_service::work(ios_));
    boost::thread service_thread(boost::bind(&Wsdd::runService, this));

    do try {
        std::clog << "Running service" << std::endl;
        ios_.run();
        service_thread.join();
        std::clog << "Service stopped" << std::endl;
    } catch (const std::exception & ex) {
        std::clog << Log::WARNING << "Something wrong - " << ex.what();
    } while (!stopped_);

    return 0;
}

void Wsdd::hello(const Hello_t & arg)
{
    std::clog << "hello("
              << (arg.xaddrs != nullptr ? *arg.xaddrs : "") << ", "
              << (arg.types != nullptr ? *arg.types : "") << ", "
              << (arg.scopes != nullptr ? arg.scopes->item : "")
              << ")" << std::endl;
}

void Wsdd::bye(const Bye_t & arg)
{
    std::clog << "bye("
              << (arg.xaddrs != nullptr ? *arg.xaddrs : "") << ", "
              << (arg.types != nullptr ? *arg.types : "") << ", "
              << (arg.scopes != nullptr ? arg.scopes->item : "")
              << ")" << std::endl;
}

void Wsdd::probe(const Probe_t & arg)
{
    std::clog << "probe("
              << (arg.types != nullptr ? *arg.types : "") << ", "
              << (arg.scopes != nullptr ? arg.scopes->item : "")
              << ")" << std::endl;
}

void Wsdd::probeMatches(const ProbeMatches_t &, const std::string &)
{
}

void Wsdd::runService()
{
    try {
        std::clog << "Sending hello" << std::endl;
        std::string address = "urn:uuid:05f1b46c-f29a-46f7-9140-e4bc00c8cea6";
        std::string xaddrs = "http://127.0.0.1/onvif/services";
        std::string types = "dn:NetworkVideoTransmitter";

        Scopes_t scopes = Scopes_t();
        scopes.item = boost::algorithm::join(scopes_, " ");
        EndpointReference_t endpoint = EndpointReference_t();
        endpoint.address = &address;

        probeMatches_.resize(1);
        probeMatches_.back().endpoint = &endpoint;
        probeMatches_.back().scopes = &scopes;
        probeMatches_.back().types = &types;
        probeMatches_.back().xaddrs = &xaddrs;
        probeMatches_.back().version = 1;

        proxy_.reset(onvifxx::RemoteDiscovery::proxy());
        proxy_->hello(probeMatches_.back());

        std::clog << "Starting the service loop" << std::endl;
        boost::scoped_ptr<Service_t> service(onvifxx::RemoteDiscovery::service());
        service->bind(this);

        while (!ios_.stopped()) {
            std::clog << "Accept" << std::endl;
            if (service->accept() == -1) {
                std::clog << Log::WARNING << "Accept failed" << std::endl;
                continue;
            }

            std::clog << "Serve" << std::endl;
            int err = service->serve();
            if (err != 0) {
                if (err != -1) {
                    std::clog << Log::WARNING << "Serve failed :"
                              << onvifxx::SoapException(*service).what() << std::endl;
                }
                std::clog << "." << std::endl;
                continue;
            }

            std::clog << "Clear" << std::endl;
            service->destroy();
        }

        proxy_->bye(probeMatches_.back());

    } catch (std::exception & ex) {
        std::clog << Log::WARNING << ex.what() << std::endl;
    }

    std::clog << "The service loop stopped" << std::endl;
    work_.reset();
}

void Wsdd::signalHandler(const Error_t & error, int signal)
{
    if (error)
        return;

    std::clog << "A signal occurred " << signal << std::endl;

    try {
        stopped_ = signal != SIGHUP;
        ios_.stop();
    } catch (std::exception & ex) {
        std::clog << Log::WARNING << "Abort on " << ex.what() << std::endl;
        std::terminate();
    }

    std::clog << "Waiting for finish of service" << std::endl;
}
