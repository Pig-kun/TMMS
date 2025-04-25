#pragma once

#include <string>
#include <vector>

namespace tmms
{
    namespace base
    {
        using std::string;
        class StringUtils
        {
        public:
            // 前后缀匹配判断函数
            static bool StartsWith(const string &s, const string &sub);
            static bool EndsWith(const string &s, const std::string &sub);
            // 文件名, 文件路径操作
            static string FilePath(const string &path);
            static string FileNameExt(const string &path);
            static string FileName(const string &path);
            static string Extension(const string &path);
            // 字符串分割
            static std::vector<string> SplitString(const string &s, const string &delimiter);
        };
    }
}