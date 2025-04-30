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

//    std::cout << "thread id : " << std::this_thread::get_id() << std::endl;

//    std::vector<EventLoop*> loops = pool.GetLoops();

//    for( auto &e : loops){

//        e->RunInLoop([&e](){
//            std::cout << "loop:" << e <<" thread id : " << std::this_thread::get_id() << std::endl;
//        }); 
//    }

//    EventLoop * loop = pool.GetNextLoop();
//    std::cout << "loop:" << loop << std::endl;
    EventLoop * loop = pool.GetNextLoop();
    std::cout << "loop:" << loop << std::endl;
    loop->RunAfter(1,[](){
        std::cout << "run after 1s now:" << tmms::base::TTime::Now() << std::endl;
    });
    loop->RunAfter(5,[](){
        std::cout << "run after 5s now:" << tmms::base::TTime::Now() << std::endl;
    });
    loop->RunEvery(1,[](){
        std::cout << "run every 1s now:" << tmms::base::TTime::Now() << std::endl;
    });
    loop->RunEvery(5,[](){
        std::cout << "run every 5s now:" << tmms::base::TTime::Now() << std::endl;
    });
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, const char ** argv){
    TestEventLoopThreadPool();
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}