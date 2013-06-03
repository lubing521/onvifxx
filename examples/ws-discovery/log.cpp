#include "log.hpp"
#include <cstring>
#include <ostream>

Log::Log(std::string ident, int facility)
{
    facility_ = facility;
    priority_ = LOG_DEBUG;

    ::strncpy(ident_, ident.c_str(), sizeof(ident_));
    ident_[sizeof(ident_) - 1] = '\0';

    ::openlog(ident_, LOG_PID, facility_);
}

Log::~Log()
{
    closelog();
}

int Log::sync()
{
    if (buffer_.length() != 0) {
        ::syslog(priority_, buffer_.c_str());

        buffer_.clear();
        priority_ = LOG_DEBUG; // default to debug for each message
    }

    return 0;
}

int Log::overflow(int c)
{
    if (c != EOF) {
        buffer_ += static_cast<char>(c);
    } else {
        sync();
    }
    return c;
}

std::ostream & operator<< (std::ostream & os, const Log::Priority & priority)
{
    static_cast<Log *>(os.rdbuf())->priority_ = priority;

    return os;
}

