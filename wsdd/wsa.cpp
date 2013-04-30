#include "wsa.hpp"
#include <Wsdd.nsmap>
#include <WsddH.h>
//#include <wsaapi.h>

#if defined(SOAP_WSA_2003)
/** Anonymous Reply/To endpoint address */
static const std::string ANONYMOUS_URI = "http://schemas.xmlsoap.org/ws/2003/03/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
static const std::string NONE_URI = "addressing/none not supported";
static const std::string FAULT_ACTION = "http://schemas.xmlsoap.org/ws/2003/03/addressing/fault";
#elif defined(SOAP_WSA_2004)
/** Anonymous Reply/To endpoint address */
static const std::string ANONYMOUS_URI = "http://schemas.xmlsoap.org/ws/2004/03/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
static const std::string NONE_URI = "addressing/none not supported";
static const std::string FAULT_ACTION = "http://schemas.xmlsoap.org/ws/2004/03/addressing/fault";
#elif defined(SOAP_WSA_2005)
/** Anonymous Reply/To endpoint address */
static const std::string ANONYMOUS_URI = "http://www.w3.org/2005/08/addressing/anonymous";
/** Specifies no Reply endpoint address (no reply) */
static const std::string NONE_URI = "http://www.w3.org/2005/08/addressing/none";
static const std::string FAULT_ACTION = "http://www.w3.org/2005/08/addressing/soap/fault";
#else
/** Anonymous Reply/To endpoint address */
static const std::string ANONYMOUS_URI = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
static const std::string NONE_URI = "addressing/none not supported";
static const std::string FAULT_ACTION = "http://schemas.xmlsoap.org/ws/2004/08/addressing/fault";
#endif


int Wsa::request(const std::string & id, const std::string & to, const std::string & action)
{
    if (allocHeader() != 0)
        return soap_->error;

    soap_->header->SOAP_WSA(MessageID) = id;
    soap_->header->SOAP_WSA(To) = to.empty() ? ANONYMOUS_URI : to;
    soap_->header->SOAP_WSA(Action) = action;
    soap_->header->SOAP_WSA(RelatesTo) = nullptr;
    soap_->header->SOAP_WSA(From) = nullptr;
    soap_->header->SOAP_WSA(ReplyTo) = nullptr;
    soap_->header->SOAP_WSA(FaultTo) = nullptr;

    return check();
}

#if defined(SOAP_WSA_2005)
/**
@fn int soap_wsa_error(struct soap *soap, SOAP_WSA(FaultCodesType) fault, const char *info)
@brief Sets SOAP Fault (sub)code for server WS-Addressing fault response.
@param soap context
@param[in] fault is one of wsa:FaultCodesType enumeration values
@param[in] info is the value of the element in the Fault detail field
@return SOAP_FAULT
*/
int
soap_wsa_error(struct soap *soap, SOAP_WSA(FaultCodesType) fault, const char *info)
{ const char *code = SOAP_WSA_(soap,FaultCodesType2s)(soap, fault);
  /* populate the SOAP Fault as per WS-Addressing spec */
  switch (fault)
  { case SOAP_WSA(InvalidAddressingHeader):
      soap_wsa_sender_fault_subcode(soap, code, "A header representing a Message Addressing Property is not valid and the message cannot be processed.", NULL);
      soap_faultdetail(soap);
      if (soap->version == 1)
      { soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
        soap->fault->detail->fault = (void*)info;
      }
      else
      { soap->fault->SOAP_ENV__Detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
        soap->fault->SOAP_ENV__Detail->fault = (void*)info;
      }
      break;
    case SOAP_WSA(InvalidAddress):
      soap_wsa_sender_fault_subcode(soap, code, "Invalid address.", NULL);
      break;
    case SOAP_WSA(InvalidEPR):
      soap_wsa_sender_fault_subcode(soap, code, "Invalid EPR.", NULL);
      break;
    case SOAP_WSA(InvalidCardinality):
      soap_wsa_sender_fault_subcode(soap, code, "Invalid cardinality of headers.", NULL);
      break;
    case SOAP_WSA(MissingAddressInEPR):
      soap_wsa_sender_fault_subcode(soap, code, "Missing EPR address.", NULL);
      break;
    case SOAP_WSA(DuplicateMessageID):
      soap_wsa_sender_fault_subcode(soap, code, "Message contains the message ID of a message already received.", NULL);
      break;
    case SOAP_WSA(ActionMismatch):
      soap_wsa_sender_fault_subcode(soap, code, "Action and SOAP action of the message do not match.", NULL);
      break;
    case SOAP_WSA(MessageAddressingHeaderRequired):
      soap_wsa_sender_fault_subcode(soap, code, "A required header representing a Message Addressing Property is not present.", NULL);
      soap_faultdetail(soap);
      if (soap->version == 1)
      { soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
        soap->fault->detail->fault = (void*)info;
      }
      else
      { soap->fault->SOAP_ENV__Detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
        soap->fault->SOAP_ENV__Detail->fault = (void*)info;
      }
      break;
    case SOAP_WSA(DestinationUnreachable):
      soap_wsa_sender_fault_subcode(soap, code, "No route can be determined to reach [destination]", NULL);
      soap_faultdetail(soap);
      if (soap->version == 1)
      { soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
        soap->fault->detail->fault = (void*)info;
      }
      else
      { soap->fault->SOAP_ENV__Detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
        soap->fault->SOAP_ENV__Detail->fault = (void*)info;
      }
      break;
    case SOAP_WSA(ActionNotSupported):
      soap_wsa_sender_fault_subcode(soap, code, "The [action] cannot be processed at the receiver.", NULL);
      soap_faultdetail(soap);
      if (soap->version == 1)
      { soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemAction);
        soap->fault->detail->fault = (void*)soap_malloc(soap, sizeof(SOAP_WSA_(,ProblemAction)));
        SOAP_WSA_(soap_default_,ProblemAction)(soap, (SOAP_WSA_(,ProblemAction)*)soap->fault->detail->fault);
        ((SOAP_WSA_(,ProblemAction)*)soap->fault->detail->fault)->Action = (char*)info;
      }
      else
      { soap->fault->SOAP_ENV__Detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemAction);
        soap->fault->SOAP_ENV__Detail->fault = (void*)soap_malloc(soap, sizeof(SOAP_WSA_(,ProblemAction)));
        SOAP_WSA_(soap_default_,ProblemAction)(soap, (SOAP_WSA_(,ProblemAction)*)soap->fault->SOAP_ENV__Detail->fault);
        ((SOAP_WSA_(,ProblemAction)*)soap->fault->SOAP_ENV__Detail->fault)->Action = (char*)info;
      }
      break;
    case SOAP_WSA(EndpointUnavailable):
      soap_wsa_receiver_fault_subcode(soap, code, "The endpoint is unable to process the message at this time.", NULL);
      soap_faultdetail(soap);
      if (soap->version == 1)
      { soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
        soap->fault->detail->fault = (void*)info;
      }
      else
      { soap->fault->SOAP_ENV__Detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
        soap->fault->SOAP_ENV__Detail->fault = (void*)info;
      }
      break;
    default:
      break;
  }
  return SOAP_FAULT;
}
#elif defined(SOAP_WSA_2003)
int
soap_wsa_error(struct soap *soap, const char *fault)
{ return soap_wsa_sender_fault_subcode(soap, NULL, fault, NULL);
}
#else
int wsaError(const Wsa * wsa, wsa__FaultSubcodeValues fault)
{
    soap * s = *wsa;
    std::string code = soap_wsa__FaultSubcodeValues2s(s, fault);
    switch (fault)
    {
    case wsa__FaultSubcodeValues__wsa__InvalidMessageInformationHeader:
        return wsa->faultSubcode(1, code, "A message information header is not valid and the message cannot be processed. The validity failure can be either structural or semantic, e.g. a [destination] that is not a URI or a [relationship] to a [message id] that was never issued.", "Invalid header");
    case wsa__FaultSubcodeValues__wsa__MessageInformationHeaderRequired:
        return wsa->faultSubcode(1, code, "A required message information header, To, MessageID, or Action, is not present.", "Missing Header QName");
    case wsa__FaultSubcodeValues__wsa__DestinationUnreachable:
        return wsa->faultSubcode(1, code, "No route can be determined to reach the destination role defined by the WS-Addressing To.", NULL);
    case wsa__FaultSubcodeValues__wsa__ActionNotSupported:
        return wsa->faultSubcode(1, code, "The [action] cannot be processed at the receiver.", s->action);
    case wsa__FaultSubcodeValues__wsa__EndpointUnavailable:
        return wsa->faultSubcode(0, code, "The endpoint is unable to process the message at this time.", NULL);
    default:
        break;
    }
    return SOAP_FAULT;
}
#endif

Wsa::Wsa(struct soap * soap) :
    soap_(soap)

{
}

Wsa::~Wsa()
{
    soap_destroy(soap_);
    soap_end(soap_);
}

std::string Wsa::randUuid()
{
    char rv[48];
    int r1, r2, r3, r4;
#ifdef WITH_OPENSSL
    r1 = soap_random;
    r2 = soap_random;
#else
    static int k = 0xFACEB00B;
    int lo = k % 127773;
    int hi = k / 127773;
# ifdef HAVE_GETTIMEOFDAY
    struct timeval tv;
    gettimeofday(&tv, NULL);
    r1 = 10000000 * tv.tv_sec + tv.tv_usec;
#else
    r1 = (int)time(NULL);
# endif
    k = 16807 * lo - 2836 * hi;
    if (k <= 0)
        k += 0x7FFFFFFF;
    r2 = k;
    k &= 0x8FFFFFFF;
    r2 += *(int*)soap_->buf;
#endif
    r3 = soap_random;
    r4 = soap_random;
    std::sprintf(rv, "urn:uuid:%8.8x-%4.4hx-4%3.3hx-%4.4hx-%4.4hx%8.8x",
            r1, (short)(r2 >> 16), (short)r2 >> 4,
            ((short)(r3 >> 16) & 0x3FFF) | 0x8000, (short)r3, r4);

    return rv;
}

int Wsa::allocHeader()
{
    soap_header(soap_);
    if (soap_->header != nullptr)
        return SOAP_OK;

    return (soap_->error = SOAP_EOM);
}


int Wsa::check() const
{
    if (soap_->header == nullptr || soap_->header->SOAP_WSA(Action).empty())
#if defined(SOAP_WSA_2005)
        return wsaError(this, soap, wsa5__MessageAddressingHeaderRequired, NULL);
#elif defined(SOAP_WSA_2003)
        return wsaError(this, "WS-Addressing header missing");
#else
        return wsaError(this, wsa__FaultSubcodeValues__wsa__MessageInformationHeaderRequired);
#endif
    return SOAP_OK;
}

//soap_wsa_fault_subcode(soap, 1, NULL, faultstring, faultdetail)
//soap_wsa_fault_subcode_action(soap, flag, faultsubcode, faultstring, faultdetail, NULL);

//soap_strdup(soap, action);
//  soap->header->SOAP_WSA(RelatesTo) = NULL;
//  soap->header->SOAP_WSA(From) = NULL;
//  soap->header->SOAP_WSA(ReplyTo) = NULL;
//  soap->header->SOAP_WSA(FaultTo) = NULL;
//  return soap_wsa_check(soap);
//}

int Wsa::addFrom(const std::string & from)
{
    if (soap_->header == nullptr)
        return SOAP_ERR;

    soap_->header->SOAP_WSA(From) = new wsa__EndpointReferenceType;
    soap_->header->SOAP_WSA(From)->soap_default(soap_);
    soap_->header->SOAP_WSA(From)->Address->__item = from;
    return SOAP_OK;
}

int Wsa::addNoReply()
{
    return addReplyTo(NONE_URI);
}

int Wsa::addReplyTo(const std::string & replyTo)
{
    if (soap_->header == nullptr)
        return SOAP_ERR;

    if (!replyTo.empty()) {
        soap_->header->SOAP_WSA(ReplyTo) = new wsa__EndpointReferenceType;
        soap_->header->SOAP_WSA(ReplyTo)->soap_default(soap_);
        soap_->header->SOAP_WSA(ReplyTo)->Address->__item = replyTo;
    }

    return SOAP_OK;
}

int Wsa::addFaultTo(const std::string & faultTo)
{
    if (soap_->header == nullptr)
        return SOAP_ERR;

    if (!faultTo.empty()) {
        soap_->header->SOAP_WSA(FaultTo) = new wsa__EndpointReferenceType;
        soap_->header->SOAP_WSA(FaultTo)->soap_default(soap_);
        soap_->header->SOAP_WSA(FaultTo)->Address->__item = faultTo;
    }

    return SOAP_OK;
}

int Wsa::addRelatesTo(const std::string & relatesTo)
{
    if (soap_->header == nullptr)
        return SOAP_ERR;

    if (!relatesTo.empty()) {
        soap_->header->SOAP_WSA(RelatesTo)->__item = relatesTo;
    }

    return SOAP_OK;
}


int Wsa::reply(const std::string & id, const std::string & action)
{
//    struct SOAP_ENV__Header * oldheader = soap_->header;
//    soap_->header = nullptr;

//    /* if endpoint address for reply is 'none' return immediately */
//    if (oldheader != nullptr && oldheader->SOAP_WSA(ReplyTo) && oldheader->SOAP_WSA(ReplyTo)->Address
//            && !strcmp(oldheader->SOAP_WSA(ReplyTo)->Address, NONE_URI))
//    {
//        return soap_send_empty_response(soap_, SOAP_OK);
//    }

//    /* allocate a new header */
//    if (alloHeader() != 0)
//        return soap_->error;

//    struct SOAP_ENV__Header * newheader = soap_->header;
//    if (oldheader != nullptr)
//        *newheader = *oldheader;

//    newheader->SOAP_WSA(MessageID) = id;
//    newheader->SOAP_WSA(Action) = action;
//    newheader->SOAP_WSA(RelatesTo) = nullptr;
//    newheader->SOAP_WSA(From) = nullptr;
//    newheader->SOAP_WSA(To) = nullptr;
//    newheader->SOAP_WSA(ReplyTo) = nullptr;
//    newheader->SOAP_WSA(FaultTo) = nullptr;


//    if (oldheader && oldheader->SOAP_WSA(MessageID)) {
//        newheader->SOAP_WSA(RelatesTo) = (SOAP_WSA_(,RelatesTo)*)soap_malloc(soap, sizeof(SOAP_WSA_(,RelatesTo)));
//        SOAP_WSA_(soap_default_,RelatesTo)(soap, newheader->SOAP_WSA(RelatesTo));
//        newheader->SOAP_WSA(RelatesTo)->__item = oldheader->SOAP_WSA(MessageID);
//    }

//    if (oldheader && oldheader->SOAP_WSA(ReplyTo) && oldheader->SOAP_WSA(ReplyTo)->Address && strcmp(oldheader->SOAP_WSA(ReplyTo)->Address, soap_wsa_anonymousURI)) {
//        newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(ReplyTo)->Address;
//        /* (re)connect to ReplyTo endpoint if From != ReplyTo */
//        if (!oldheader->SOAP_WSA(From) || !oldheader->SOAP_WSA(From)->Address || strcmp(oldheader->SOAP_WSA(From)->Address, oldheader->SOAP_WSA(ReplyTo)->Address)) {
//            struct soap *reply_soap = soap_copy(soap);
//            if (reply_soap) {
//                soap_copy_stream(reply_soap, soap);
//                soap_clr_omode(reply_soap, SOAP_ENC_MIME | SOAP_ENC_DIME | SOAP_ENC_MTOM);
//                soap_free_stream(soap); /* prevents close in soap_connect() below */
//                newheader->SOAP_WSA(FaultTo) = oldheader->SOAP_WSA(FaultTo);
//                soap->header = newheader;
//                if (soap_connect(soap, newheader->SOAP_WSA(To), newheader->SOAP_WSA(Action))) {
//                    int err;
//                    soap_copy_stream(soap, reply_soap);
//#if defined(SOAP_WSA_2005)
//                    err = soap_wsa_error(soap, SOAP_WSA(DestinationUnreachable), newheader->SOAP_WSA(To));
//#elif defined(SOAP_WSA_2003)
//                    err = soap_wsa_error(soap, "WS-Addessing destination unreachable");
//#else
//                    err = soap_wsa_error(soap, SOAP_WSA(DestinationUnreachable));
//#endif
//                    soap_free_stream(reply_soap);
//                    soap_end(reply_soap);
//                    soap_free(reply_soap);
//                    soap->header = NULL;
//                    return err;
//                }
//                if (soap_valid_socket(reply_soap->socket)) {
//                    soap_send_empty_response(reply_soap, SOAP_OK);	/* HTTP ACCEPTED */
//                    soap_closesock(reply_soap);
//                }
//                soap_free_stream(reply_soap);
//                soap_end(reply_soap);
//                soap_free(reply_soap);
//                data->fresponse = soap->fresponse;
//                soap->fresponse = soap_wsa_response;	/* response will be a POST */
//            }
//        }
//    } else if (oldheader && oldheader->SOAP_WSA(From))
//        newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(From)->Address;
//    else
//        newheader->SOAP_WSA(To) = (char*)soap_wsa_anonymousURI;

//    soap->header = newheader;
//    soap->action = newheader->SOAP_WSA(Action);

    return SOAP_OK;
}

int Wsa::faultSubcode(int flag, const std::string & faultsubcode, const std::string & faultstring,
                      const std::string & faultdetail) const
{
    struct SOAP_ENV__Header * oldheader = soap_->header;
//    if (oldheader != nullptr && oldheader->SOAP_WSA(FaultTo) && !strcmp(oldheader->SOAP_WSA(FaultTo)->Address, NONE_URI))
//        return soap_send_empty_response(soap_, SOAP_OK);


//  /* allocate a new header */
//    soap_->header = nullptr;
//    if (allocHeader(soap))
//        return soap->error;


//    newheader = soap->header;
//    soap_default_SOAP_ENV__Header(soap, newheader); /* remove/clear SOAP Header */

//    /* check header */
//    if (oldheader && oldheader->SOAP_WSA(MessageID)) {
//        newheader->SOAP_WSA(RelatesTo) = (SOAP_WSA_(,RelatesTo)*)soap_malloc(soap, sizeof(SOAP_WSA_(,RelatesTo)));
//        SOAP_WSA_(soap_default_,RelatesTo)(soap, newheader->SOAP_WSA(RelatesTo));
//        newheader->SOAP_WSA(RelatesTo)->__item = oldheader->SOAP_WSA(MessageID);
//    }

//    /* header->wsa__MessageID = "..."; */
//    newheader->SOAP_WSA(Action) = (char*)soap_wsa_faultAction;
//    if (oldheader && oldheader->SOAP_WSA(FaultTo) && oldheader->SOAP_WSA(FaultTo)->Address && strcmp(oldheader->SOAP_WSA(FaultTo)->Address, soap_wsa_anonymousURI)) {
//        newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(FaultTo)->Address;
//        /* (re)connect to FaultTo endpoint if From != FaultTo */

//        if (!oldheader->SOAP_WSA(From) || !oldheader->SOAP_WSA(From)->Address || strcmp(oldheader->SOAP_WSA(From)->Address, oldheader->SOAP_WSA(FaultTo)->Address)) {
//            soap->keep_alive = 0;
//            soap_send_empty_response(soap, SOAP_OK);	/* HTTP ACCEPTED */

//            if (soap_connect(soap, newheader->SOAP_WSA(To), newheader->SOAP_WSA(Action)))
//                return soap->error = SOAP_STOP;	/* nowhere to go */

//            soap_set_endpoint(soap, newheader->SOAP_WSA(To));

//            if (action)
//                soap->action = (char*)action;
//            else
//                soap->action = newheader->SOAP_WSA(Action);

//            data->fresponse = soap->fresponse;
//            soap->fresponse = soap_wsa_response;	/* response will be a POST */
//        }
//    } else if (oldheader && oldheader->SOAP_WSA(From)) {
//        newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(From)->Address;
//    } else {
//        newheader->SOAP_WSA(To) = (char*)soap_wsa_anonymousURI;
//    }

//  soap->header = newheader;


  if (flag)
    return soap_sender_fault_subcode(soap_, faultsubcode.c_str(), faultstring.c_str(), faultdetail.c_str());

  return soap_receiver_fault_subcode(soap_, faultsubcode.c_str(), faultstring.c_str(), faultdetail.c_str());
}


