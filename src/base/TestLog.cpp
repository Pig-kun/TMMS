#include "LogStream.h"
#include "Logger.h"

using namespace tmms::base;

void testLog(){
    LOG_TRACE << "This is a trace log";
    LOG_DEBUG << "This is a debug log";
    LOG_INFO << "This is an info log";
    LOG_WARN << "This is a warn log";
    LOG_ERROR << "This is an error log";
}

int main(){
    tmms::base::g_logger = new Logger();
    tmms::base::g_logger->SetLogLevel(kWarn);
    testLog();
    tmms::base::g_logger->SetLogLevel(kInfo);
    testLog();
    return 0;
}

