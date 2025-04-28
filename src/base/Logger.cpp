#include "Logger.h"
#include <iostream>

using namespace tmms::base;

Logger::Logger(const FileLogPtr &log)
:log_(log){

}

void Logger::SetLogLevel(const LogLevel &level){
    level_ = level;
}

LogLevel Logger::GetLogLevel() const{
    return level_;
}

void Logger::Write(const std::string &msg){
    if(log_){
        log_->WriteLog(msg);
    }else{
        std::cout << msg;
    }
}