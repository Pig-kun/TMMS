#pragma once
/*
    网络编程经常要用的 IP 和端口，传递两个值比较麻烦
    IP 和端口经常需要进行转换成其他形式
    有时候需要对地址进行分类检测
    InetAddress 类主要方便存储 IP 和端口信息，提供地址相关的操作
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <bits/socket.h>

namespace tmms {
    namespace network {
        class InetAddress {
        public:
            InetAddress(const std::string &ip, uint16_t port, bool bv6=false);
            InetAddress(const std::string &host, bool is_v6=false);
            InetAddress() = default;
            ~InetAddress() = default; 

            // 赋值函数
            void SetHost(const std::string &host);
            void SetAddr(const std::string &addr);
            void SetPort(uint16_t port);
            void SetIsIPV6(bool is_v6);

            // 取值函数
            const std::string &IP() const;
            uint32_t IPv4() const;
            std::string ToIpPort() const;
            uint16_t Port() const;
            void GetSockAddr(struct sockaddr *addr) const;

            // 测试函数
            bool IsIpV6() const;
            bool IsWanIp() const;
            bool IsLanIp() const;
            bool IsLoopbackIp() const;

            static void GetIpAndPort(const std::string &host, std::string &ip, std::string &port);

        private:
            // 私有取值
            uint32_t IPv4(const char *ip) const;
            // 私有成员变量
            std::string addr_;
            std::string port_;
            bool is_ipv6_{false};
        };
    }
}