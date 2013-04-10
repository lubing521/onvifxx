#include <onvifxx/onvifxx.hpp>
#include <onvif.nsmap>

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

SoapException::SoapException(soap * s)
{
    char buf[1024];
    soap_sprint_fault(s, buf, sizeof(buf));
    message().assign(buf);
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

} // namespace onvifxx
