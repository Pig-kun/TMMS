#include "TimingWheel.h"
#include "network/base/Network.h"
using namespace tmms::network;

TimingWheel::TimingWheel():wheels_(4){
    wheels_[kTimingWheelSecond].resize(60);
    wheels_[kTimingWheelMinute].resize(60);
    wheels_[kTimingWheelHour].resize(24);
    wheels_[kTimingWheelDay].resize(30);
}

TimingWheel::~TimingWheel(){

}

void TimingWheel::InsertEntry(uint32_t delay, EntryPtr entrPtr){
    if(delay <= 0){
        entrPtr.reset();
    }
    if(delay < kTimingMinute){
        InsertSecondEntry(delay, entrPtr);
    }
    else if(delay < kTimingHour){
        InsertMinuteEntry(delay, entrPtr);
    }
    else if(delay < kTimingDay){
        InsertHourEntry(delay, entrPtr);
    }
    else{
        auto day = delay / kTimingDay;
        if(day > 30){
            NETWORK_ERROR << "NO SUPPORT ! day is too long. day:" << day;
            return;
        }
        InsertDayEntry(delay, entrPtr);
    }
}

void TimingWheel::OnTimer(int64_t now){
    if(last_ts_ == 0){
        last_ts_ = now;
    }
    if(now - last_ts_ < 1000){
        return;
    }
    last_ts_ = now;
    tick_++;
    PopUp(wheels_[kTimingWheelSecond]);
    if(tick_ % kTimingMinute == 0){
        PopUp(wheels_[kTimingWheelMinute]);
    }else if(tick_ % kTimingHour == 0){
        PopUp(wheels_[kTimingWheelHour]);
    }else if(tick_ % kTimingDay == 0){
        PopUp(wheels_[kTimingWheelDay]);
    }
}
void TimingWheel::PopUp(Wheel &bq){
    WheelEntry tmp;
    bq.front().swap(tmp);
    bq.pop_front();
    bq.push_back(WheelEntry());
}

void TimingWheel::RunAfter(double delay, const Func &cb){
    CallbackEntryPtr cbEntry = std::make_shared<CallbackEntry>([cb](){
        cb();
    }); 
    InsertEntry(delay, cbEntry);
}

void TimingWheel::RunAfter(double delay, Func &&cb){
    CallbackEntryPtr cbEntry = std::make_shared<CallbackEntry>([cb](){
        cb();
    }); 
    InsertEntry(delay, cbEntry);
}

void TimingWheel::RunEvery(double inerval, const Func &cb){
    CallbackEntryPtr cbEntry = std::make_shared<CallbackEntry>([this, inerval, cb](){
        cb();
        RunEvery(inerval, cb);
    }); 
    InsertEntry(inerval, cbEntry);
}

void TimingWheel::RunEvery(double inerval, Func &&cb){
    CallbackEntryPtr cbEntry = std::make_shared<CallbackEntry>([this, inerval, cb](){
        cb();
        RunEvery(inerval, cb);
    }); 
    InsertEntry(inerval, cbEntry);
}

void TimingWheel::InsertSecondEntry(uint32_t delay, EntryPtr entryPtr){
    wheels_[kTimingWheelSecond][delay - 1].emplace(entryPtr);
}

void TimingWheel::InsertMinuteEntry(uint32_t delay, EntryPtr entryPtr){
    
    auto minute = delay / kTimingMinute;
    auto second = delay % kTimingMinute;
    CallbackEntryPtr newEntryPtr = std::make_shared<CallbackEntry>([this, second, entryPtr](){
        InsertEntry(second, entryPtr);
    });
    wheels_[kTimingWheelMinute][minute - 1].emplace(newEntryPtr);
}

void TimingWheel::InsertHourEntry(uint32_t delay, EntryPtr entryPtr){
    auto hour = delay / kTimingHour;
    auto second = delay % kTimingHour;
    CallbackEntryPtr newEntryPtr = std::make_shared<CallbackEntry>([this, second, entryPtr](){
        InsertEntry(second, entryPtr);
    });
    wheels_[kTimingWheelHour][hour - 1].emplace(newEntryPtr);
}

void TimingWheel::InsertDayEntry(uint32_t delay, EntryPtr entryPtr){
    auto day = delay / kTimingDay;
    auto second = delay % kTimingDay;
    CallbackEntryPtr newEntryPtr = std::make_shared<CallbackEntry>([this, second, entryPtr](){
        InsertEntry(second, entryPtr);
    });
    wheels_[kTimingWheelDay][day - 1].emplace(newEntryPtr);
}