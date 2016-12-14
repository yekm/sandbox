#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>
#include <vector>
#include <stdexcept>


class Socket
{
public:
    explicit Socket();
    virtual ~Socket();

    void read(std::vector<unsigned char> &);
    ssize_t write(const std::vector<unsigned char> &);
    ssize_t write(const std::string &);
    template <typename T>
    ssize_t write(T begin, T end)
    {
        auto size = ::send(m_socket, static_cast<const void*>(&*begin), std::distance(begin, end), 0);
        if (size < 0)
            throw std::runtime_error("send error");
        return size;
    }

    void sendfile(const std::string & name);
    void setopt(int name, int val);
    void bind_any(int port);
    void listen(int bl = 5);
    Socket accept(struct sockaddr_in *s);
    Socket(Socket &&);
protected:
    explicit Socket(int s);
private:
    Socket(const Socket &) = delete;
    Socket& operator= (const Socket &) = delete;
    int m_socket;

    // keep some state for sanity checks like dont listen on unboud socket
};

#endif // SOCKET_HPP
