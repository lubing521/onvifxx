#ifndef WSDD_HPP
#define WSDD_HPP

#include <onvifxx/remotediscovery.hpp>

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/scoped_ptr.hpp>

#include <vector>

class Wsdd : public onvifxx::RemoteDiscovery
{
    typedef boost::system::error_code Error_t;
    typedef onvifxx::Proxy<onvifxx::RemoteDiscovery> Proxy_t;
    typedef onvifxx::Service<onvifxx::RemoteDiscovery> Service_t;

public:
    Wsdd();
    virtual ~Wsdd();

    int exec(bool daemonize);

    virtual void hello(std::string * xaddrs, std::string * types, Scopes_t * scopes);
    virtual void bye(std::string * xaddrs, std::string * types, Scopes_t * scopes);
    virtual ProbeMatches_t probe(std::string * types, Scopes_t * scopes);


private:
    void runService();
    void signalHandler(const Error_t & error, int signal);

private:
    bool stopped_;
    boost::asio::io_service ios_;
    boost::asio::signal_set signals_;
    boost::scoped_ptr<boost::asio::io_service::work> work_;
    std::vector<std::string> scopes_;
};

#endif // WSDD_HPP
