#pragma once
#include <cstdint>
#include <functional>
#include <memory>

// 定时任务

namespace tmms
{
    namespace base
    {
        class Task;
        using TaskPtr = std::shared_ptr<Task>;
        using TaskCallback = std::function<void (const TaskPtr &)>;
        class Task:public std::enable_shared_from_this<Task>
        {
        public:
            Task(const TaskCallback &cb, int64_t interval);
            Task(const TaskCallback &&cb, int64_t interval);

            void Run();
            void Restart();
            int64_t When() const { return when_; }

        private:
            int64_t interval_{0};
            int64_t when_{0};
            TaskCallback cb_; 
        };
    }
}