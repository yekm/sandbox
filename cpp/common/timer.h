#ifndef TIMER_H
#define TIMER_H

#include <ostream>
#include <iomanip>


namespace common
{

class Timer : public timespec
{
public:
    Timer();
    Timer restart();

    friend std::ostream & operator << (std::ostream & outs, const Timer & a);

    Timer operator-(const Timer & a);
    Timer & operator-=(const Timer & a);
    Timer operator+(const Timer & a);
    Timer & operator+=(const Timer & a);

    virtual srd::string to_str() const;
};

std::ostream & operator << (std::ostream & outs, const Timer & a)
{
    Timer t;
    t -= a;
    outs << t.tv_sec << "." << std::setfill('0') << std::setw(9) << t.tv_nsec;
    return outs;
}


} // namespace common

#endif // TIMER_H
