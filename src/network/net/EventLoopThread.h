#pragma once
/*
    EventLoopThread 通过创建一个std::thread 线程来运行EventLoop
    EventLoopThread 只运行一个EventLoop
    EventLoopThread 保证RventLoop的生命周期和std：：thread相同
*/
#include "base/NonCopyable.h"
#include "EventLoop.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

namespace tmms
{
    namespace network{
        class EventLoopThread:public base::NonCopyable{
        public:
            EventLoopThread();
            ~EventLoopThread();

            void Run();
            
            EventLoop * Loop() const;

            std::thread &Thread();

        private:
            void StartEventLoop();

            EventLoop * loop_{nullptr};
            std::thread thread_;
            bool running_{false};
            std::mutex lock_;
            std::condition_variable condition_;
            std::once_flag once_;
            std::promise<int> promise_loop;
        };
    }
}