#ifndef ONVIFXX_WSDD_HPP
#define ONVIFXX_WSDD_HPP

#if defined(SOAP_WSA_2003)
# define SOAP_WSA(member) wsa3__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa3__##member
# define SOAP_WSA__(prefix,member) prefix##wsa3__##member
#elif defined(SOAP_WSA_2004)
# define SOAP_WSA(member) wsa4__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa4__##member
# define SOAP_WSA__(prefix,member) prefix##wsa4__##member
#elif defined(SOAP_WSA_2005)
# define SOAP_WSA(member) wsa5__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa5__##member
# define SOAP_WSA__(prefix,member) prefix##wsa5__##member
#else
# define SOAP_WSA(member) wsa__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa__##member
# define SOAP_WSA__(prefix,member) prefix##wsa__##member
#endif

#include <onvifxx/onvifxx.hpp>

class Wsa
{
public:
    Wsa(struct soap * soap);
    ~Wsa();

    std::string randUuid();

    int allocHeader();
    int check() const;

    int addFrom(const std::string & from);
    int addNoReply();
    int addReplyTo(const std::string & replyTo);
    int addFaultTo(const std::string & faultTo);
    int addRelatesTo(const std::string & relatesTo);

    int reply(const std::string & id, const std::string & action);
    int request(const std::string & to, const std::string & action);


private:
    soap * soap_;
};

#endif // ONVIFXX_WSDD_HPP
