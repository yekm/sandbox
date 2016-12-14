#include <iostream>
#include <vector>
#include <stdlib.h>
#include <chrono>

#include "tcpserversocket.hpp"
#include "threadpool.hpp"

// enables the multithreading
#define MT

void process(Socket && s)
{
    std::vector<unsigned char> read_buf(2048);
    s.read(read_buf);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // hard working
    s.write(std::string("HTTP/1.0 200 OK\n\nblabla"));
}

int main(int argc, const char *argv[])
{
    int port = 8888;
    if (argc > 1)
        port = atoi(argv[1]); // wow, very c++, much oop

#ifdef MT
    ThreadPool<Socket> tp(10, [](Socket && s)
    {
        std::cout << "working " << std::this_thread::get_id() << std::endl;
        process(std::move(s));
    });
#endif

    TcpServerSocket ss(port);
    while (true)
    {
        Socket s = ss.accept();

#ifdef MT
        tp.submit(std::move(s));
#else
        //std::cout << std::string(read_buf.begin(), read_buf.end());
        process(std::move(s));
#endif
    }
    return 0;
}
