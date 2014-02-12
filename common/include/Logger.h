#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

namespace fmitcp {

    class Logger {
    public:

        enum LogMessageType {
            DEBUG = 1,
            NETWORK = 2
        };

        Logger();
        virtual ~Logger();
        virtual void log(LogMessageType type, const char * format, ...);
    };

};

#endif
