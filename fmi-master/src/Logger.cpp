#include "Logger.h"
#include "stdio.h"

Logger::Logger(){

}

Logger::~Logger(){

}

void Logger::log(LogMessageType type, const char * format, ...){
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}
