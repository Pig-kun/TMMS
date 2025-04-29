#pragma once
#include <vector>
#include <sys/epoll.h>
#include <memory>
#include <unordered_map>   
#include "Event.h"
/*
    IO就绪事件监听
    IO事件处理
    事件管理
*/
namespace tmms
{
namespace network
    {
        using EventPtr = std::shared_ptr<Event>;
        
        class EventLoop
        {
        public:
            EventLoop();
            ~EventLoop();

            void Loop();
            void Quit();

            void AddEvent(const EventPtr &event);
            void DelEvent(const EventPtr &event);

            bool EnableEventWriting(const EventPtr &event, bool enable);
            bool EnableEventReading(const EventPtr &event, bool enable);
        private:
            bool looping_{false};
            int epoll_fd_{-1};
            std::vector<struct epoll_event> epoll_events_;
            std::unordered_map<int, EventPtr> events_;
        };
    }
}