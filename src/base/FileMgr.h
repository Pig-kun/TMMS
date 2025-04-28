#pragma once
/*
    日志文件申请与释放，    日志文件的切分检测
*/
#include <string>
#include "FileLog.h"
#include "Singleton.h"
#include <memory>
#include <unordered_map>
#include <mutex>

namespace tmms
{
    namespace base
    {


        class FileMgr:public NonCopyable
        {
        public:
            FileMgr() = default;
            ~FileMgr() = default;
            // 检查切分条件是否成立
            void OnCheck();
            // 返回一个FileLog
            FileLogPtr GetFileLog(const std::string &file_name);
            // 删除
            void RemoveFileLog(const FileLogPtr &filelog);
            // 切分
            void RotateDays(const FileLogPtr &filelog);
            void RotateHours(const FileLogPtr &filelog);
            void RotateMinutes(const FileLogPtr &filelog);
        private:
            std::unordered_map<std::string, FileLogPtr> logs_;
            std::mutex lock_;
            int last_day_{-1};
            int last_hour_{-1};
            int last_minute_{-1};
            int last_year_{-1};
            int last_month_{-1};
        };
    }
}

#define sFileMgr tmms::base::Singleton<tmms::base::FileMgr>::Instance()