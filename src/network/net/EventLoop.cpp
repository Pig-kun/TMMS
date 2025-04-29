#include "EventLoop.h"
#include "network/base/Network.h"
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace tmms::network;

// 一个线程只需有一个事件循环最多
static thread_local EventLoop * t_local_eventloop = nullptr;

EventLoop::EventLoop():epoll_fd_(::epoll_create(1024)), epoll_events_(1024){
    if(t_local_eventloop){
        NETWORK_ERROR << "EventLoop already exists in this thread!!!!!";
        exit(-1);
    }
    t_local_eventloop = this;
}

EventLoop::~EventLoop(){
    Quit();
}

void EventLoop::Loop(){
    looping_ = true;
    while (looping_) {
        memset(&epoll_events_[0], 0x00, sizeof(struct epoll_event)*epoll_events_.size());
        auto ret = ::epoll_wait(epoll_fd_,
                                 (struct epoll_event*)&epoll_events_[0],
                                  static_cast<int>(epoll_events_.size()), 
                                  -1);
        if(ret > 0){
            for( int i = 0; i < ret; i++){  
                struct epoll_event &ev = epoll_events_[i];
                if(ev.data.fd <= 0){
                    NETWORK_ERROR << "epoll wait error(非法fd). fd:" << ev.data.fd;
                    continue;
                }
                auto iter = events_.find(ev.data.fd);
                if(iter == events_.end()){
                    NETWORK_ERROR << "epoll wait error(查不到). fd:" << ev.data.fd;
                    continue;
                }
                EventPtr & event = iter->second;

                if(ev.events&EPOLLERR){

                    int error = 0;
                    socklen_t len = sizeof(error);
                    getsockopt(event->Fd(), SOL_SOCKET, SO_ERROR, &error, &len);

                    event->OnError(strerror(error));
                }else if((ev.events&EPOLLHUP) && !(ev.events&EPOLLIN)){
                    event->OnClose();
                }else if(ev.events & (EPOLLIN | EPOLLPRI)){
                    event->OnRead();
                }else  if(ev.events & EPOLLOUT){
                    event->OnWrite();
                }
            }

            if(ret == epoll_events_.size()){
                epoll_events_.resize(epoll_events_.size() * 2);
            }
        }else if(ret == 0){

        }else if(ret < 0){
            NETWORK_ERROR << "epoll wait error.error:" << errno;
        }
    }
}

void EventLoop::Quit(){
    looping_ = false;
}

void EventLoop::AddEvent(const EventPtr &event){
    auto iter = events_.find(event->Fd());
    if(iter != events_.end()){
        return;
    }
    event->event_ |= kEventRead;
    events_[event->Fd()] = event;

    struct epoll_event ev;
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event->fd_, &ev);
}

void EventLoop::DelEvent(const EventPtr &event){
    auto iter = events_.find(event->Fd());
    if(iter == events_.end()){
        return;
    }
    events_.erase(iter);

    struct epoll_event ev;
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, event->fd_, &ev);
}

bool EventLoop::EnableEventWriting(const EventPtr &event, bool enable){
    auto iter = events_.find(event->Fd());
    if(iter == events_.end()){
        NETWORK_ERROR << "Event not found in event loop--fd:" << event->Fd();
        return false;
    }
    
    if(enable){
        event->event_ |= kEventWrite;
    }else{
        event->event_ &= ~kEventWrite;
    }

    struct epoll_event ev;
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, event->fd_, &ev);
    return true;
}

bool EventLoop::EnableEventReading(const EventPtr &event, bool enable){
    auto iter = events_.find(event->Fd());
    if(iter == events_.end()){
        NETWORK_ERROR << "Event not found in event loop--fd:" << event->Fd();
        return false;
    }
    
    if(enable){
        event->event_ |= kEventRead;
    }else{
        event->event_ &= ~kEventRead;
    }

    struct epoll_event ev;
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, event->fd_, &ev);
    return true;
}