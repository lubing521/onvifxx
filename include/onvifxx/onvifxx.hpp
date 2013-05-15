#ifndef ONVIFXX_H
#define ONVIFXX_H

#include <memory>
#include <vector>
#include <string>
#include <iostream>

#define UNUSED(x) (void)x

struct soap;

#if __GNUC_MINOR__ < 6
    #define noexcept throw()

    const class {                       // this is a const object...
    public:
        template<class T>               // convertible to any type
        operator T*() const             // of null non-member
        { return 0; }                   // pointer...
        template<class C, class T>      // or any type of null
        operator T C::*() const         // member pointer...
        { return 0; }
    private:
        void operator&() const;         // whose address can't be taken
    } nullptr = {};                     // and whose name is nullptr
#endif

namespace onvifxx {


static const std::string WSDD_URL = "soap.udp://239.255.255.250:3702";

class Exception : public std::exception
{
public:
    Exception();
    Exception(const char * msg);
    virtual ~Exception() throw();
    virtual const char * what() const throw();

protected:
    std::string & message();

private:
    std::string msg_;

};

class SoapException : public Exception
{
public:
    SoapException(soap * s);

    int code() const;

private:
    int code_;
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
T * proxy();

template<class T>
T * service();

class ServiceBase
{
protected:
    struct Engine
    {
        virtual	int serve() = 0;
        virtual	int accept() = 0;

        virtual	void print() = 0;
    };

    virtual void run(Engine * engine);
};

template<class T>
class Service :
        private ServiceBase
{

public:
    struct Engine : T, ServiceBase::Engine
    {
        virtual	int serve() { return T::serve(); }
        virtual	int accept() { return T::accept(); }

        virtual	void print()
        {
            std::cerr << "!!!!!!!!!!!!!!!!!" << std::endl;
        }
    };

    virtual void run(Engine * engine = nullptr)
    {
        ServiceBase::run(dynamic_cast<ServiceBase::Engine *>(this));
    }
};


//template<class T>
//class Pimpl
//{
//public:
//    Pimpl() : p_(new T) { }
//    T * impl() { return p_.get(); }
//private:
//    std::shared_ptr<T> p_;
//};

} // namespace onvifxx

#endif //ONVIFXX_H
