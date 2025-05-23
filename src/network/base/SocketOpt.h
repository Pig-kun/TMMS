#pragma once
/*
    SOCKET操作使用的接口和参数多
    InetAddress使用的时候，需要进行转换
    封装接口便于使用
*/
#include "InetAddress.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>

namespace tmms{
    namespace network{
        using InetAddressPtr = std::shared_ptr<InetAddress>;
        class SockerOpt{
        public:
            SockerOpt(int sock, bool v6=false);
            ~SockerOpt() = default;
            
            // 创建接口
            static int CreateNonblockingTcpSocket(int family);
            static int CreateNonblockingUdpSocket(int family);

            // 服务器
            int BindAddress(const InetAddress &localaddr);
            int Listen();
            int Accept(InetAddress *peeraddr);
            
            // 客户端
            int Connect(const InetAddress &addr);
        
            // 获取地址
            InetAddressPtr GetLocalAddr();
            InetAddressPtr GetPeerAddr();

            // 使能函数
            void SetTcpNoDelay(bool on);
            void SetReuseAddr(bool on);
            void SetReusePort(bool on);
            void SetKeepAlive(bool on);
            void SetNonBlocking(bool on);
        
        private:
            int sock_{-1};
            bool is_v6_{false};
            
        };
    }
}