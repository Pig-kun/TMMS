#pragma once

namespace tmms
{
    namespace base
    {
        class NonCopyable
        {// 实现单例模式,基类 
        protected:
            NonCopyable(){}
            ~NonCopyable(){}
            NonCopyable(const NonCopyable &) = delete;
            NonCopyable &operator=(const NonCopyable &) = delete;
        };
    }
}