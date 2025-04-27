#pragma once

#include "Task.h"
#include "Singleton.h"
#include "NonCopyable.h"
#include <unordered_set>
#include <mutex>
/*
    时间任务管理器，功能：
    1. 存放所有的定时任务   4. 检测每个任务是否到点执行
    2. 添加定时任务         5. 执行任务
    3. 删除定时任务         6. 全局唯一
*/ 

namespace tmms
{
    namespace base
    {
        class TaskMgr: public NonCopyable
        
        {
        public:
            TaskMgr() = default;
            ~TaskMgr() = default;

             void OnWork();
             bool Add(TaskPtr &task);
             bool Del(TaskPtr &task);

        private:
            std::unordered_set <TaskPtr> tasks_;
            std::mutex lock_;
        };
    }
    #define sTaskMgr tmms::base::Singleton<tmms::base::TaskMgr>::Instance()
}