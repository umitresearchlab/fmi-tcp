#include "Logger.h"
#include "stdio.h"

using namespace fmitcp;

Logger::Logger(){
    m_filter = ~0; // Log everything
}

Logger::~Logger(){

}

void Logger::log(Logger::LogMessageType type, const char * format, ...){
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

void Logger::setFilter(int filter){
    m_filter = filter;
}

int Logger::getFilter() const {
    return m_filter;
}
