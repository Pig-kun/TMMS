#include "EventLoopThreadPool.h"
#include <pthread.h>

using namespace tmms::network;
namespace{
    void bind_cpu(std::thread &t, int n){
        cpu_set_t cpu;

        CPU_ZERO(&cpu);
        CPU_SET(n, &cpu);

        pthread_setaffinity_np(t.native_handle(), sizeof(cpu), &cpu);
    }
}

EventLoopThreadPool::EventLoopThreadPool(int thread_num, int start, int cpus){
    
    if(thread_num <= 0){
        thread_num = 1;
    }
    
    for(int i = 0; i < thread_num; i++){
        threads_.emplace_back(std::make_shared<EventLoopThread>());
        if(cpus > 0){
            int n = (start+i) %cpus;
            bind_cpu(threads_.back()->Thread(), n);
        }
    }
}

EventLoopThreadPool::~EventLoopThreadPool(){

}

std::vector<EventLoop*> EventLoopThreadPool::GetLoops() const{

    std::vector<EventLoop*> results;
    for(auto &t:threads_){
        results.emplace_back(t->Loop());
    }
    return results;
}

EventLoop * EventLoopThreadPool::GetNextLoop(){
    
    int index = loop_index_;
    loop_index_++;
    return threads_[index % threads_.size()]->Loop();

}

size_t EventLoopThreadPool::Size(){
    return threads_.size();
}

void EventLoopThreadPool::Start(){
    for(auto &t:threads_){
        t->Run();
    }
}