#include "LogStream.h"
#include "Logger.h"
#include "FileLog.h"
#include "FileMgr.h"
#include "TaskMgr.h"
#include "Task.h"   
#include <thread>
#include "TTime.h"
#include <iostream>

using namespace tmms::base;
std::thread t;

void testLog(){

    t = std::thread([](){
        while(true){
            LOG_DEBUG << "This is a debug log!!!now:" << TTime::NowMS();
            LOG_INFO << "This is an info log!!!now:" << TTime::NowMS();
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
        }
    });
}

int main(){

    FileLogPtr log = sFileMgr->GetFileLog("test.log");

    log->SetRotate(kRotateMinute);

    tmms::base::g_logger = new Logger(log);
    tmms::base::g_logger->SetLogLevel(kTrace);
    
    TaskPtr task4 = std::make_shared<Task>([](const TaskPtr &task){
        sFileMgr->OnCheck();
        task->Restart();
    }, 1000);

    sTaskMgr->Add(task4);
    
    testLog();
    while(1){
        sTaskMgr->OnWork();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}

