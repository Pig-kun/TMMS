#include "network/base/InetAddress.h"
#include <string>
#include <iostream>

using namespace tmms::network;

int main(int argc, const char **argv){
    std::string  host;
    while(std::cin >> host){
        InetAddress addr(host);
        std::cout << "host:" << host << std::endl
                    << "ip:" << addr.IP() << std::endl
                    << "port:" << addr.Port() << std::endl
                    << "ipv4:" << addr.IPv4() << std::endl
                    << "is_ipv6:" << addr.IsIpV6() << std::endl
                    << "is_wan_ip:" << addr.IsWanIp() << std::endl
                    << "is_lan_ip:" << addr.IsLanIp() << std::endl
                    << "is_loopback_ip:" << addr.IsLoopbackIp() << std::endl;
    }
    return 0;
}