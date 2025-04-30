#pragma once
/*
    事件循环定时任务，采用时间轮方式实现
    有若干个轮，秒，分，时，天。。。轮存在容器 vector里
    每个轮有若干个槽，是一个双端队列
    槽上存的元素是任务，存在unodered_set里。
*/
#include <vector>
#include <unordered_set>
#include <deque>
#include <memory>
#include <functional>
#include <cstdint>

namespace tmms{
    namespace network{

        using EntryPtr = std::shared_ptr<void>;
        using WheelEntry = std::unordered_set<EntryPtr>;
        using Wheel = std::deque<WheelEntry>;
        using Wheels = std::vector<Wheel>;
        using Func = std::function<void()>;

        const int kTimingMinute = 60;
        const int kTimingHour = 3600;
        const int kTimingDay = 86400;

        enum TimingWheelType{
            kTimingWheelSecond = 0,
            kTimingWheelMinute,
            kTimingWheelHour,
            kTimingWheelDay,
        };

        class CallbackEntry{
        public:
            CallbackEntry(const Func &cb): cb_(cb) {}
            ~CallbackEntry(){
                if(cb_){
                    cb_();
                }
            }
        private:
            Func cb_;
        };

        using CallbackEntryPtr = std::shared_ptr<CallbackEntry>;

        class TimingWheel{
        public:
            TimingWheel();
            ~TimingWheel();

            void InsertEntry(uint32_t delay, EntryPtr entrPtr);
            void OnTimer(int64_t now);
            void PopUp(Wheel &bq);      
            void RunAfter(double delay, const Func &cb);
            void RunAfter(double delay, Func &&cb);
            void RunEvery(double inerval, const Func &cb);
            void RunEvery(double inerval, Func &&cb);

        private:
            Wheels wheels_;
            int64_t last_ts_{0};
            uint64_t tick_{0};

            void InsertSecondEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertMinuteEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertHourEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertDayEntry(uint32_t delay, EntryPtr entryPtr);
        };
    }
}