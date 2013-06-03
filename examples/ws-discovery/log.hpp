#ifndef LOG_HPP
#define LOG_HPP

#include <streambuf>
#include "syslog.h"

class Log :
    public std::basic_streambuf<char, std::char_traits<char> >
{
public:
    enum Priority {
        EMERGENCY    = LOG_EMERG,   // system is unusable
        ALERT        = LOG_ALERT,   // action must be taken immediately
        CRITICAL     = LOG_CRIT,    // critical conditions
        ERROR        = LOG_ERR,     // error conditions
        WARNING      = LOG_WARNING, // warning conditions
        NOTICE       = LOG_NOTICE,  // normal, but significant, condition
        INFO         = LOG_INFO,    // informational message
        DEBUG        = LOG_DEBUG    // debug-level message
    };

    explicit Log(std::string ident, int facility);
    ~Log();

protected:
    int sync();
    int overflow(int c);

private:
    friend std::ostream & operator<< (std::ostream & os, const Priority & priority);
    std::string buffer_;
    int facility_;
    int priority_;
    char ident_[50];
};

std::ostream & operator<< (std::ostream & os, const Log::Priority & priority);

#endif // LOG_HPP
