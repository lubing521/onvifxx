#include <onvifxx/onvifxx.hpp>
#include <Onvif.nsmap>

extern "C" {

auto namespaces = Onvif_namespaces;

} // extern "C"

namespace onvifxx {

Exception::Exception()
{
}

Exception::Exception(const char * msg) :
    msg_(msg)
{

}

std::string & Exception::message()
{
    return msg_;
}

const char * Exception::what() const noexcept
{
    return msg_.c_str();
}

SoapException::SoapException(soap * s) :
    code_(-1)
{
    if (s != nullptr) {
        char buf[1024];
        soap_sprint_fault(s, buf, sizeof(buf));
        message().assign(buf);

        code_ = s->errnum;
    }
}

int SoapException::code() const
{
    return code_;
}

UnixException::UnixException(int code)
{
    if (code == 0)
        code = errno;

    char buf[1024];
    message().assign(strerror_r(code, buf, sizeof(buf) - 1));

    code_ = code;
}

int UnixException::code() const
{
    return code_;
}

void ServiceBase::run(Engine * engine)
{
    for (;;)
    {
        if (!soap_valid_socket(engine->accept()))
            break;

        if (!soap_valid_socket(engine->serve()))
            break;
    }
}


} // namespace onvifxx
