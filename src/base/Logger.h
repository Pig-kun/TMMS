#pragma once
#include "NonCopyable.h"
#include "FileLog.h"
#include <string>
#include <memory>

namespace tmms
{
    namespace base{
        enum LogLevel{
             kTrace,
             kDebug,
             kInfo,
             kWarn,
             kError,
             kMsxNumOfLogLevel,
        };

        class Logger:public NonCopyable
        {
        public:
            Logger(const FileLogPtr &log);
            ~Logger() = default;

            void SetLogLevel(const LogLevel &level);
            LogLevel GetLogLevel() const;
            void Write(const std::string &msg);
        private:
            LogLevel level_{kDebug};
            FileLogPtr log_;
        };
    }
}