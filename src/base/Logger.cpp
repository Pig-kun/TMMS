#include "Logger.h"
#include <iostream>

using namespace tmms::base;

void Logger::SetLogLevel(const LogLevel &level){
    level_ = level;
}

LogLevel Logger::GetLogLevel() const{
    return level_;
}

void Logger::Write(const std::string &msg){
    std::cout << msg << std::endl;
}