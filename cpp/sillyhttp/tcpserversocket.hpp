#ifndef TCPSERVERSOCKET_HPP
#define TCPSERVERSOCKET_HPP

#include "socket.hpp"

class TcpServerSocket : protected Socket
{
public:
    TcpServerSocket(int port);
    ~TcpServerSocket();

    Socket accept();
};

#endif // TCPSERVERSOCKET_HPP
