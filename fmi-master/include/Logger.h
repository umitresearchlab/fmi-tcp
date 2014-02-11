#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

enum LogMessageType {
    DEBUG = 1,
    NETWORK = 2
};

class Logger {
public:
    Logger();
    virtual ~Logger();
    virtual void log(LogMessageType type, const char * format, ...);
};

#endif
