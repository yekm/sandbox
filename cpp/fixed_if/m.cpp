#include <iostream>
#include "fastcomp.h"

#include <ratio>
#include <chrono>
#include <limits>

int a = 10;
int b = 5;
unsigned long int c = 1;

void test1()
{
    c = 0;
    while (c < 1e9)
        if (b < 10)
        {
            c++;
        }
}

void test2()
{
    c = 0;
    while (c < 1e9)
    {
        P( if(b < 10), c++, ; )
    }
}

int main(void)
{
//    std::cout << (void*)&__executable_start << "\n" << (void*)&__etext << std::endl;
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    test1();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << time_span.count() << " seconds. original if()\n";

    t1 = high_resolution_clock::now();
    test2();
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << time_span.count() << " seconds. patched if()\n";

    return 0;
}
