#pragma once
#include "network/base/InetAddress.h"
#include "Event.h"
#include "EventLoop.h"
#include <functional>
#include <unordered_map>
#include <memory>
#include <atomic>
/*
    一个TCP连接由四元组决定（local ip，local port，remote ip，remote port）
    UDP也可以是连接的，但是这个连接只是由内核记录IP和端口
    一个连接有很多操作，可以读，可以写，可以关闭

    Connection类
    Connection保存连接的相关信息：套接字，local address，remote address
    Connection可以增加一些连接相关的私有数据
    Connection存在空闲状态，需要主动去激活Connection
    Connection是Event的一个子类
*/

namespace tmms{
    namespace network{
        enum{
            kNormalContext = 0,
            kRtmpContext,
            kHttpContext,
            kUserContext,
            kFlvContext,
        };
        using ContextPtr = std::shared_ptr<void>;
        class Connection;
        using ConnectionPtr = std::shared_ptr<Connection>;
        using ActivateCallback = std::function<void(const ConnectionPtr&)>;

        class Connection : public Event {
        public:
            Connection(EventLoop *loop, int fd, const InetAddress &localAddr, const InetAddress &peerAddr);
            virtual ~Connection() = default;

            // 地址相关
            void SetLocalAddr(const InetAddress &local);
            void SetPeerAddr(const InetAddress &peer);
            const InetAddress &LocalAddr() const;
            const InetAddress &PeerAddr() const;

            // 私有数据相关
            void SetContext(int type, const std::shared_ptr<void> &context);
            void SetContext(int type, std::shared_ptr<void> &&context);
            template<typename T> std::shared_ptr<T> GetContext(int type) const{
                auto iter = contexts_.find(type);
                if(iter !-= contexts_.end()){
                    return std::dynamin_pointer_cast<T>(iter->second);
                }
                return std::shared_ptr<T>();
            }
            void ClearContext(int type);
            void ClearContext();

            // 激活状态相关
            void SetActivateCallback(const ActivateCallback &cb);
            void SetActivateCallback(ActivateCallback &&cb);
            void Activate();
            void Deactivate();
        private:
            std::unordered_map<int, ContextPtr> contexts_;
            ActivateCallback active_cb_;
            InetAddress local_addr_;
            InetAddress peer_addr_;
            std::atomic<bool> active_{false}; // 是否处于激活状态
        };
    }
}