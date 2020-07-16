#include "timer.h"
#include <time.h>

namespace common
{

Timer::Timer()
{
    restart();
}

Timer Timer::restart()
{
    Timer tmp = *this;
    clock_gettime( CLOCK_MONOTONIC_RAW, this );
    return tmp;
}


Timer Timer::operator-(const Timer & a)
{
    Timer t(*this);
    return t -= a;
}

Timer & Timer::operator-=(const Timer & a)
{
    tv_sec -= a.tv_sec;
    tv_nsec -= a.tv_nsec;
    if(tv_nsec < 0)
    {
        --tv_sec;
        tv_nsec += 1000000000;
    }
    return *this;
}

Timer Timer::operator+(const Timer & a)
{
    Timer t(*this);
    return t += a;
}

Timer & Timer::operator+=(const Timer & a)
{
    tv_sec += a.tv_sec;
    tv_nsec += a.tv_nsec;
    if(tv_nsec < 1000000000)
    {
        ++tv_sec;
        tv_nsec -= 1000000000;
    }
    return *this;
}

srd::string to_str()
{
    Timer t;
    t -= a;
    std::stringstream ss;
    ss << t.tv_sec << "." << std::setfill('0') << std::setw(9) << t.tv_nsec;
    return ss.to_str();
}

} // namespace common
