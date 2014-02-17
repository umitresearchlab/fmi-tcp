#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

namespace fmitcp {

    class Logger {

    private:
        int m_filter;

    public:

        enum LogMessageType {
            DEBUG,
            NETWORK
        };

        Logger();
        virtual ~Logger();
        virtual void log(LogMessageType type, const char * format, ...);
        virtual void setFilter(int filter);
        virtual int getFilter() const;
    };

};

#endif
