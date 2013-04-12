#ifndef WSDD_HPP
#define WSDD_HPP

#include <string>
#include <vector>

namespace onvifxx {

class Wsdd
{
    struct BaseMatch
    {
        struct Scopes
        {
            std::string item;
            std::string matchBy;
        };

        typedef unsigned short Port_t;
        typedef std::pair<std::string, Port_t> Endpoint_t;

        Endpoint_t endpoint;
        std::string types;
        Scopes scopes;
        std::string xaddrs;
        uint version;
    };

public:
    static const uint APP_MAX_DELAY = 15;

    struct ProbeMatch : BaseMatch
    {
        typedef std::vector<ProbeMatch> List_t;
    };


    struct ResolveMatch : BaseMatch
    {
        typedef std::vector<ResolveMatch> List_t;
    };


    Wsdd();
    virtual ~Wsdd();

    enum Mode { MODE_MANAGED, MODE_ADHOC };
    enum To { TO_DP, TO_TS };

    static void setInstanceId(uint instanceId);

    static void setSequenceId(const std::string & sequenceId);

    std::string genUuid();

    void hello(Mode mode, const std::string & endpoint, const std::string & messageId,
               const std::string & relatesTo, const std::string & endpointRef,
               const std::string & types, const std::string & scopes, const std::string & matchBy,
               const std::string & xaddrs, uint metadataVersion);

    void bye(Mode mode, const std::string & endpoint, const std::string & messageId,
             const std::string & endpointRef, const std::string & types, const std::string & scopes,
             const std::string & matchBy,    const std::string & xaddrs, uint metadataVersion);

    void probe(To to, const std::string & endpoint, const std::string & messageId,
               const std::string & replyTo, const std::string & types, const std::string & scopes,
               const std::string & matchBy);

    void resolve(To to, const std::string & endpoint, const std::string & messageId,
                 const std::string & replyTo, const std::string & endpointRef);

    void probeMatches(const std::string & endpoint, const std::string & messageId,
                      const std::string & relatesTo, const std::string & to,
                      const ProbeMatch::List_t & matches);

    void resolveMatches(const std::string & endpoint, const std::string & messageId,
                        const std::string & relatesTo, const std::string & to,
                        const std::string & endpointRef, const std::string & types,
                        const std::string & scopes, const std::string & matchBy,
                        const std::string & xaddrs, uint metadataVersion);

    ProbeMatch::List_t getProbeMatches();
    ResolveMatch getResolveMatch();

    int listen(int timeout);

private:
    struct Impl;
    Impl * impl_;
};

} // namespace onvifxx

#endif // WSDD_HPP
