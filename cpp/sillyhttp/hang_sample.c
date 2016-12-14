#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{
    int port = 8888;
    if (argc > 1)
        port = atoi(argv[1]); // wow, very c++, much oop

    int s = socket(AF_INET, SOCK_STREAM, 0);
    int one = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    s_addr.sin_port = htons(port);

    if (bind(s, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
        error(-1, errno, "Can't bind");
    if (listen(s, 5) == -1)
        error(-1, errno, "Can't listen");

    while (1)
    {
        struct sockaddr_in sin;
        socklen_t s_len = sizeof(struct sockaddr_in);
        int client_socket = accept(s, (struct sockaddr *)&sin, &s_len);
        if (client_socket == -1)
            error(-1, errno, "Can't accept");

        char buf[2048];
        size_t srecv = recv(client_socket, buf, 2048, 0);
        if (srecv < 0)
            error(-1, errno, "Can't recv");

        char a[] = "HTTP/1.0 200 OK\n\nblabla";
        size_t ssend = send(client_socket, a, sizeof(a), 0);
        if (ssend < 0)
            error(-1, errno, "Can't send");
        close(client_socket);
    }

    return 0;
}
