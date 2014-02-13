#include "Logger.h"
#include "stdio.h"

using namespace fmitcp;

Logger::Logger(){

}

Logger::~Logger(){

}

void Logger::log(Logger::LogMessageType type, const char * format, ...){
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}
