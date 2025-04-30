#include "InetAddress.h"
#include  "Network.h"
#include <cstring>

using namespace tmms::network;

void InetAddress::GetIpAndPort(const std::string &host, std::string &ip, std::string &port){
    auto pos = host.find_last_of(':', 0);
    if(pos != std::string::npos){
        ip = host.substr(0, pos);
        port = host.substr(pos + 1);
    }else{
        ip = host;
    }
}

InetAddress::InetAddress(const std::string &ip, uint16_t port, bool bv6=false)
:addr_(ip),port_(std::to_string(port)),is_ipv6_(bv6)
{}

InetAddress::InetAddress(const std::string &host, bool is_v6=false){
    GetIpAndPort(host, addr_, port_);
    is_ipv6_ = is_v6;
}

// 赋值函数
void InetAddress::SetHost(const std::string &host){
    GetIpAndPort(host, addr_, port_);
}

void InetAddress::SetAddr(const std::string &addr){
    addr_ = addr;
}
void InetAddress::SetPort(uint16_t port){
    port_ = std::to_string(port);
}
void InetAddress::SetIsIPV6(bool is_v6){
    is_ipv6_ = is_v6;
}

// 取值函数
const std::string & InetAddress::IP() const{
    return addr_;
}
// 私有取值
uint32_t InetAddress::IPv4(const char *ip) const{
    struct sockaddr_in addr_in;
    memset(&addr_in, 0x00, sizeof(sockaddr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = 0; 
    if(::inet_pton(AF_INET, ip, &addr_in.sin_addr) < 0){
        NETWORK_ERROR << "ipv4 ip :" << ip << " is error";
        return 0;
    }
    return ntohl(addr_in.sin_addr.s_addr);
}
//
uint32_t InetAddress::IPv4() const{
    return IPv4(addr_.c_str());
}

std::string InetAddress::ToIpPort() const{
    std::stringstream ss;
    ss << addr_ << ":" << port_;
    return ss.str();
}

uint16_t InetAddress::Port() const{
    return atoi(port_.c_str());
}

void InetAddress::GetSockAddr(struct sockaddr *saddr) const{
    if(is_ipv6_){
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)saddr;
        memset(&addr_in6, 0x00, sizeof(sockaddr_in6));

        addr_in6->sin6_family = AF_INET6;
        addr_in6->sin6_port = htons(Port());
        if(::inet_pton(AF_INET6, addr_.c_str(), &addr_in6->sin6_addr) < 0){

        }
        return;
    }

    struct sockaddr_in *addr_in = (struct sockaddr_in *)saddr;
    memset(&addr_in, 0x00, sizeof(sockaddr_in));

    addr_in->sin_family = AF_INET;
    addr_in->sin_port = htons(Port());
    if(::inet_pton(AF_INET, addr_.c_str(), &addr_in->sin_addr) < 0){

    }
}

// 测试函数
bool InetAddress::IsIpV6() const{}
bool InetAddress::IsWanIp() const{}
bool InetAddress::IsLanIp() const{}
bool InetAddress::IsLoopbackIp() const{}