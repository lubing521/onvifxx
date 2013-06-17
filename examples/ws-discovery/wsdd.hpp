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

    virtual void hello(const Hello_t & arg);
    virtual void bye(const Bye_t & arg);
    virtual ProbeMatches_t probe(const Probe_t & arg);
    //virtual void probeMatches(const ProbeMatches_t & arg, const std::string & relatesTo);

private:
    static bool isMatched(const std::string & left, const std::string & right);

    void runService();
    void signalHandler(const Error_t & error, int signal);

private:
    bool stopped_;
    boost::asio::io_service ios_;
    boost::asio::signal_set signals_;
    boost::scoped_ptr<boost::asio::io_service::work> work_;

    std::vector<std::string> scopes_;
    ProbeMatches_t probeMatches_;
    boost::scoped_ptr<Proxy_t> proxy_;
};

#endif // WSDD_HPP
