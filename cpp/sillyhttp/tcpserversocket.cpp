#include "tcpserversocket.hpp"

#include <stdexcept>

TcpServerSocket::TcpServerSocket(int port)
{
    setopt(SO_REUSEADDR, 1);
    bind_any(port);
    listen();
}

TcpServerSocket::~TcpServerSocket()
{
}

Socket TcpServerSocket::accept()
{
    struct sockaddr_in c_addr;
    return Socket::accept(&c_addr);
}
