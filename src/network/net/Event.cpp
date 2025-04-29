#include "Event.h"
#include "EventLoop.h"
#include <unistd.h>
using namespace tmms::network;

Event::Event(EventLoop *loop):loop_(loop){

}

Event::Event(EventLoop *loop, int fd):loop_(loop), fd_(fd){

}

Event::~Event(){
    if(fd_>0){
        ::close(fd_);
        fd_ = -1;
    }
}


bool Event::EnableWriting(bool enable){
    return loop_->EnableEventWriting(shared_from_this(), enable);
}

bool Event::EnableReading(bool enable){
    return loop_->EnableEventReading(shared_from_this(), enable);
}

int Event::Fd() const{
    return fd_;
}