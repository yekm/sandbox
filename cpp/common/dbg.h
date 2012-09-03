#ifndef DBG_H

#define DBG_H

struct Dbg
{
    Dbg()
    {
        std::cout << "[ ";
    }
    ~Dbg()
    {
        std::cout << "]\n" << std::flush;
    }
    template<class T>
    Dbg& operator<< (T d)
    {
        std::cout << d << " ";
        return *this;
    }

    Dbg& operator<< (char* d)
    {
        return (*this) << (void*)d;
    }
    Dbg& operator<< (const char *&d)
    {
        return (*this) << (void*)d;
    }
};

#ifdef DEBUG
    #define D(x) Dbg()
#else
    #define D(x) if (false) Dbg()
#endif

#endif /* end of include guard: DBG_H */

