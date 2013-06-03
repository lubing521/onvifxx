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

    scopes_.push_back("onvif://www.onvif.org/type");
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
        std::clog << "Running service loop" << std::endl;
        ios_.run();
        service_thread.join();
        std::clog << "Service loop exited" << std::endl;
    } catch (const std::exception & ex) {
        std::clog << Log::WARNING << "Something wrong - " << ex.what();
    } while (!stopped_);

    return 0;
}

void Wsdd::hello(std::string * xaddrs, std::string * types, Scopes_t * scopes)
{
    std::clog << "hello("
              << (xaddrs != nullptr ? *xaddrs : "") << ", "
              << (types != nullptr ? *types : "") << ", "
              << (scopes != nullptr ? scopes->first : "")
              << ")" << std::endl;
}

void Wsdd::bye(std::string * xaddrs, std::string * types, Scopes_t * scopes)
{
    std::clog << "bye("
              << (xaddrs != nullptr ? *xaddrs : "") << ", "
              << (types != nullptr ? *types : "") << ", "
              << (scopes != nullptr ? scopes->first : "")
              << ")" << std::endl;
}

Wsdd::ProbeMatches_t Wsdd::probe(std::string * types, Scopes_t * scopes)
{
    ProbeMatches_t rv;
    if (types && types->find_first_of("dn:NetworkVideoTransmitter") == std::string::npos)
        return rv;

    if (scopes != nullptr && !scopes->first.empty()) {
        boost::tokenizer<> tokens(scopes->first);
        if (std::find_first_of(tokens.begin(), tokens.end(), scopes_.begin(), scopes_.end()) == tokens.end())
            return rv;
    }

    rv.push_back("http://127.0.0.1/onvif/device_service");

    return rv;
}

void Wsdd::runService()
{
    try {
        std::clog << "Sending hello" << std::endl;
        std::string xaddrs = "http://127.0.0.1/onvif/services";
        std::string types = "dn:NetworkVideoTransmitter";
        Scopes_t scopes = Scopes_t(boost::algorithm::join(scopes_, " "), nullptr);
        boost::scoped_ptr<Proxy_t> proxy(onvifxx::RemoteDiscovery::proxy());
        proxy->hello(&xaddrs, &types, &scopes);

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

        proxy->bye(&xaddrs, &types, &scopes);

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
