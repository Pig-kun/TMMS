#include "Connection.h"
using namespace tmms::network;

Connection::Connection(EventLoop *loop, int fd, const InetAddress &localAddr, const InetAddress &peerAddr)
:Event(loop, fd), local_addr_(localAddr), peer_addr_(peerAddr)
{

}