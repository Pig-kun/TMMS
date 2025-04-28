#pragma once
/*
    日志文件FileLog
    append方式打开
    负责把日志信息写到文件
    负责完成日志文件的切割
*/

#include <string>
#include <memory>

namespace tmms
{
    namespace base
    {
        enum RotateType
        {
            kRotateNone = 0, // 不切分
            kRotateMinute,
            kRotateHour, // 按小时切分
            kRotateDay,     // 按天切分
        };

        class FileLog
        {
        public:
            FileLog() = default;
            ~FileLog() = default;

            // 打开日志文件
            bool Open(const std::string &filePath);
            // 写日志
            size_t WriteLog(const std::string &msg);
            // 日志切分
            void Rotate(const std::string &file);
            // 设置日志的切分类型
            void SetRotate(RotateType type);
            // 返回日志切分类型
            RotateType GetRotateType() const;
            // 返回文件长度
            int64_t FileSize() const;
            // 路径访问函数
            std::string FilePath() const;

        private:
            int fd_;
            std::string file_path_;
            RotateType rotate_type_{kRotateNone};
        };
        using FileLogPtr = std::shared_ptr<FileLog>;
    }
}