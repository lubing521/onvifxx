#ifndef ONVIFXX_H
#define ONVIFXX_H

#include <memory>
#include <string>

struct soap;

namespace onvifxx {

class Exception : public std::exception
{
public:
    Exception();
    Exception(const char * msg);
    virtual const char* what() const noexcept;

protected:
    std::string & message();

private:
    std::string msg_;

};

class SoapException : public Exception
{
public:
    SoapException(soap * s);
};

class UnixException : public Exception
{
public:
    UnixException(int code = 0);

    int code() const;

private:
    int code_;
};

template<class T>
class Pimpl
{
public:
    Pimpl() : p_(new T) { }
    T * impl() { return p_.get(); }
private:
    std::shared_ptr<T> p_;
};

class Server;

} // namespace onvifxx

#endif //ONVIFXX_H
