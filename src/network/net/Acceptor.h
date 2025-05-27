#pragma once
/*
    Tcp服务器由Accept接受新连接
    Accept每次从接受队列中取第一个请求，接受队列中都是完成了三次握手的请求
    设置SO_REUSERPORT后，多线程可以同时监听同一个地址和端口

    ******非阻塞Accept设计******
    非阻塞的监听sockfd，会马上返回，没有连接就会返回EAGAIN
    非阻塞的监听套接字配合epoll使用
    有新链接，epoll返回读就绪时间
    边缘触发模式下，一次读事件要一直读到返回EAGAIN错误为止
    Acceptor是一个Event的子类，主要处理读事件
*/
#include "network/base/InetAddress.h"
#include "network/base/SocketOpt.h"
#include "network/net/Event.h"
#include "network/net/EventLoop.h"

namespace tmms {
    namespace network {
        
        using AcceptCallback = std::function<void(int sock, const InetAddress &addr)>;
        class Acceptor : public Event {
        public:
            Acceptor(EventLoop *loop, const InetAddress &addr);
            ~Acceptor();

            void SetAcceptCallback(const AcceptCallback &cb);
            void SetAcceptCallback(AcceptCallback &&cb);
            void Start();
            void Stop();
            void OnRead() override;
            void OnError(const std::string &msg) override;
            void OnClose() override;
        private:
            void Open();
            InetAddress addr_;
            AcceptCallback accept_cb_;
            SocketOpt *socket_opt_{nullptr}; 
        };       
    }
}