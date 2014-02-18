#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <stdarg.h>
#include <stdlib.h>

namespace fmitcp {

    class Logger {

    private:
        int m_filter;
        std::string m_prefix;

    public:

        enum LogMessageType {
            DEBUG,
            NETWORK,
            ERROR,
        };

        Logger();
        virtual ~Logger();
        virtual void log(LogMessageType type, const char * format, ...);
        virtual void setFilter(int filter);
        virtual int getFilter() const;
        void setPrefix(std::string prefix);
    };

};

#endif
