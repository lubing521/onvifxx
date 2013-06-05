#include "wsdd.hpp"
#include "log.hpp"

std::string baseName(const char * path)
{
    std::string rv;

    while (*path != '\0') {
        if (*path != '/') {
            rv += *path;
        } else {
            rv.clear();
        }

        ++path;
    }

    return rv;
}

int main(int argc, char * argv[])
{
    bool daemonize = !(argc == 2 && argv[1] == std::string("-D"));
    if (daemonize)
        std::clog.rdbuf(new Log(baseName(argv[0]), LOG_DAEMON));
    return Wsdd().exec(daemonize);
}
