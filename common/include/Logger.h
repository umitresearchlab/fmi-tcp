#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

namespace fmitcp {

    class Logger {
    public:

        enum LogMessageType {
            DEBUG,
            NETWORK
        };

        Logger();
        virtual ~Logger();
        virtual void log(LogMessageType type, const char * format, ...);
    };

};

#endif
