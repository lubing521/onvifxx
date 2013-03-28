#include "wsdd.h"
#include "wsdd.nsmap"

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

void wsdd_event_ProbeMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ProbeMatchesType *matches)
{
    EventHandlerMap * ehm = (EventHandlerMap*)soap->user;
    ehm->probeMatches(soap, InstanceId, SequenceId, MessageNumber, MessageID, RelatesTo, matches);
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
