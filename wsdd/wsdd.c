#include "wsdd.h"

void wsdd_event_Hello(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion)
{
    EventHandlerMap * ehm = (EventHandlerMap*)soap->user;
    ehm->hello(soap, InstanceId, SequenceId, MessageNumber, MessageID, RelatesTo, EndpointReference, Types, Scopes, MatchBy, XAddrs, MetadataVersion);
}

void wsdd_event_Bye(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int *MetadataVersion)
{
    EventHandlerMap * ehm = (EventHandlerMap*)soap->user;
    ehm->bye(soap, InstanceId, SequenceId, MessageNumber, MessageID, RelatesTo, EndpointReference, Types, Scopes, MatchBy, XAddrs, MetadataVersion);
}

soap_wsdd_mode wsdd_event_Probe(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *Types, const char *Scopes, const char *MatchBy, struct wsdd__ProbeMatchesType *matches)
{
    EventHandlerMap * ehm = (EventHandlerMap*)soap->user;
    return ehm->probe(soap, MessageID, ReplyTo, Types, Scopes, MatchBy, matches);
}

void wsdd_event_ProbeMatches(struct soap *s, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ProbeMatchesType *matches)
{
    EventHandlerMap * ehm = (EventHandlerMap*)s->user;
    fprintf(stderr, "%p\n", s->user);
    ehm->probeMatches(s, InstanceId, SequenceId, MessageNumber, MessageID, RelatesTo, matches);
}

soap_wsdd_mode wsdd_event_Resolve(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *EndpointReference, struct wsdd__ResolveMatchType *match)
{
    EventHandlerMap * ehm = (EventHandlerMap*)soap->user;
    return ehm->resolve(soap, MessageID, ReplyTo, EndpointReference, match);
}

void wsdd_event_ResolveMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ResolveMatchType *match)
{
    EventHandlerMap * ehm = (EventHandlerMap*)soap->user;
    ehm->resolveMatches(soap, InstanceId, SequenceId, MessageNumber, MessageID, RelatesTo, match);
}


struct Namespace * wsdd_namespaces()
{
    static SOAP_NMAC struct Namespace rv[] =
    {
        {"s", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
        {"e", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
        {"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
        {"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
        {"a", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL, NULL},
        {"d", "http://schemas.xmlsoap.org/ws/2005/04/discovery", NULL, NULL},
        {"dn", "http://www.onvif.org/ver10/network/wsdl", NULL, NULL},
        {NULL, NULL, NULL, NULL}
    };

    return rv;
}
