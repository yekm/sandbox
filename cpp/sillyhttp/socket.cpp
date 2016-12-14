#include "socket.hpp"

#include <unistd.h>

#include <stdexcept>

// rename to InetSocket and pass type
// or rename to tcpsocket
Socket::Socket()
    : m_socket(::socket(AF_INET, SOCK_STREAM, 0))
{
    if (m_socket == 0)
        throw std::runtime_error("can't create socket ");
    // add strerror somethere ^^
}

Socket::~Socket()
{
    if (m_socket != 0)
        ::close(m_socket);
}

void Socket::read(std::vector<unsigned char> & buf)
{
    auto size = ::recv(m_socket, &buf[0], buf.size(), 0);
    if (size < 0)
        throw std::runtime_error("recv error");

    //if (size == 0)
    //    throw std::runtime_error("peer disconnected");

    buf.resize(size);
    // or return size and dont touch the buf
}

ssize_t Socket::write(const std::vector<unsigned char> & buf)
{
    return write(buf.begin(), buf.end());
}

ssize_t Socket::write(const std::string &buf)
{
    return write(buf.begin(), buf.end());
}

void Socket::setopt(int name, int val)
{
    ::setsockopt(m_socket, SOL_SOCKET, name, &val, sizeof(val));
}

// pass s_addr.sin_addr.s_addr and rename to bind
void Socket::bind_any(int port)
{
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    s_addr.sin_port = htons(port);

    if (::bind(m_socket, reinterpret_cast<sockaddr *>(&s_addr), sizeof(s_addr)) == -1)
        throw std::runtime_error("Can't bind");
}

void Socket::listen(int bl)
{
    if (::listen(m_socket, bl) == -1)
        throw std::runtime_error("Can't listen");
}

Socket Socket::accept(struct sockaddr_in * s)
{
    socklen_t s_len = sizeof(struct sockaddr_in);
    int client_socket = ::accept(m_socket, reinterpret_cast<sockaddr *>(s), &s_len);
    if (client_socket == -1)
        throw std::runtime_error("Can't accept");
    return Socket(client_socket);
}

Socket::Socket(Socket && o)
{
    m_socket = o.m_socket;
    o.m_socket = 0;
}

Socket::Socket(int s)
    : m_socket(s)
{
}
