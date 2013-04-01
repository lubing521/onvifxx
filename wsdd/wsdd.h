#ifndef WSDD_H
#define WSDD_H

#include "wsddapi.h"

typedef struct
{
    void (*hello)(struct soap *, unsigned int, const char *, unsigned int, const char *, const char *, const char *, const char *, const char *, const char *, const char *, unsigned int);
    void (*bye)(struct soap *, unsigned int, const char *, unsigned int, const char *, const char *, const char *, const char *, const char *, const char *, const char *, unsigned int *);
    soap_wsdd_mode (*probe)(struct soap *, const char *, const char *, const char *, const char *, const char *, struct wsdd__ProbeMatchesType *);
    void (*probeMatches)(struct soap *, unsigned int, const char *, unsigned int, const char *, const char *, struct wsdd__ProbeMatchesType *);
    soap_wsdd_mode (*resolve)(struct soap *, const char *, const char *, const char *, struct wsdd__ResolveMatchType *);
    void (*resolveMatches)(struct soap *, unsigned int, const char *, unsigned int, const char *, const char *, struct wsdd__ResolveMatchType *);
} EventHandlerMap;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Namespace * wsdd_namespaces();

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // WSDD_H
