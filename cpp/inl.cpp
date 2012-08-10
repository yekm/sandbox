#include <iostream>

struct dbg
{
    dbg()
    {
        std::cout << "[";
    }
    ~dbg()
    {
        std::cout << "] ";
    }
    template<class T>
    dbg& operator<< (T d)
    {
        std::cout << d << " ";
        return *this;
    }
/*    template<int N>
    dbg& operator<< (const char d[N])
    {
        std::cout << "!!!" << d;
        return (*this);
    }*/
    dbg& operator<< (char* d)
    {
        return (*this) << (void*)d;
    }
    dbg& operator<< (const char *&d)
    {
        return (*this) << (void*)d;
    }
};

struct flusher
{
    ~flusher()
    {
        std::cout << std::endl << std::flush;
    }
};

int main(int argc, const char *argv[])
{
    flusher f;
    char * c;
    c = "abc";
    const char * cc = c;
    dbg() << "hi" << c << cc;
    return 0;
}

