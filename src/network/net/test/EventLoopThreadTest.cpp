#include "network/net/EventLoopThread.h"
#include "network/net/EventLoopThreadPool.h"
#include "network/net/EventLoop.h"
#include "base/TTime.h"
#include "network/net/PipeEvent.h"
#include <iostream>
#include <thread>

using namespace tmms::network;

EventLoopThread eventloop_thread;
std::thread th;

void TestEventLoopThread(){
    eventloop_thread.Run();
    EventLoop * loop = eventloop_thread.Loop();

    if(loop){
        std::cout << "loop:" << loop <<std::endl;
        PipeEventPtr pipe_event = std::make_shared<PipeEvent>(loop);
        loop->AddEvent(pipe_event);
        int64_t test = 12345;
        pipe_event->Write((const char*)&test, sizeof(test));
        th = std::thread([&pipe_event](){
            
            while(1){
                std::this_thread::sleep_for(std::chrono::seconds(1));
                int64_t now = tmms::base::TTime::NowMS();
                pipe_event->Write((const char*)&now, sizeof(now));
            }
        });
        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void TestEventLoopThreadPool(){
    EventLoopThreadPool pool(2, 0, 2);

    pool.Start();

    std::vector<EventLoop*> loops = pool.GetLoops();

    for( auto &e : loops){
        std::cout << "loop:" << e << std::endl;
    }

    EventLoop * loop = pool.GetNextLoop();
    std::cout << "loop:" << loop << std::endl;
    loop = pool.GetNextLoop();
    std::cout << "loop:" << loop << std::endl;
}

int main(int argc, const char ** argv){
    TestEventLoopThreadPool();
    return 0;
}