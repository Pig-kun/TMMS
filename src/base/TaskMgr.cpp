#include "TaskMgr.h"
#include "TTime.h"

using namespace tmms::base;

void TaskMgr::OnWork(){
    std::lock_guard<std::mutex> lock(lock_);
    for(auto iter = tasks_.begin(); iter != tasks_.end();){
        auto task = *iter;
        int64_t now = TTime::NowMS();
        if(task->When() <= now){
            task->Run();
            if(task->When() < now){
                iter = tasks_.erase(iter);
                continue;
            }
        }
        ++iter;
    }
}

bool TaskMgr::Add(TaskPtr &task){
    std::lock_guard<std::mutex> lock(lock_);
    auto iter = tasks_.find(task);
    if(iter != tasks_.end()){
        return false;
    }
    tasks_.emplace(task);
}

bool TaskMgr::Del(TaskPtr &task){
    std::lock_guard<std::mutex> lock(lock_);
    tasks_.erase(task);
}