#include "FileLog.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
using namespace tmms::base;

// 打开日志文件
bool FileLog::Open(const std::string &filePath){
    file_path_ = filePath;
    int fd =  ::open(file_path_.c_str(), O_CREAT | O_APPEND | O_WRONLY, DEFFILEMODE);
    if(fd < 0){
        std::cout << "open file log error: " << file_path_ << std::endl;
        return false;
    }
    fd_ = fd;
    return true;
}

// 写日志
size_t FileLog::WriteLog(const std::string &msg){
    int fd = fd_ == -1?1:fd_;
    return ::write(fd, msg.data(), msg.size());
}

// 日志切分
void FileLog::Rotate(const std::string &file){
    if(file_path_.empty()){
        return;
    }
    int ret = ::rename(file_path_.c_str(), file.c_str());
    if(ret != 0){
        std::cerr << "rename file log error: " << file_path_ << "new:" << file;
        return;
    }
    int fd =  ::open(file_path_.c_str(), O_CREAT | O_APPEND | O_WRONLY, DEFFILEMODE);
    if(fd < 0){
        std::cout << "open file log error: " << file << std::endl;
        return;
    }
    ::dup2(fd, fd_);
    close(fd);

}

// 设置日志的切分类型
void FileLog::SetRotate(RotateType type){
    rotate_type_ = type;
}

// 返回日志切分类型
RotateType FileLog::GetRotateType() const{
    return rotate_type_;
}

// 返回文件长度
int64_t FileLog::FileSize() const{
    return ::lseek(fd_, 0, SEEK_END);
}

// 路径访问函数
std::string FileLog::FilePath() const{
    return file_path_;
}