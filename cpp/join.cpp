#include <iostream>
#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

#include "common/timer.h"

std::string test_v(int x)
{
    std::vector<std::string> v;
    for (int i=0; i<x; ++i)
    {
        v.push_back(boost::lexical_cast<std::string>(i));
    }
    return boost::algorithm::join(v, ", ");
}

std::string test_a(int x)
{
    std::string res;
    for (int i=0; i<x; ++i)
    {
        res += boost::lexical_cast<std::string>(i) + ", ";
    }
    res.resize(res.size()-2);
    return res;
}

std::string test_s(int x)
{
    std::ostringstream oss;
    for (int i=0; i<x; ++i)
    {
        oss << boost::lexical_cast<std::string>(i) << ", ";
    }
    oss.seekp(-2, std::ios_base::end);
    oss << " ";
    return oss.str();
}

int main(int argc, const char *argv[])
{
    size_t sum = 0;
    common::Timer t1;
    for (int i=0; i<1e4; ++i)
        sum += test_v(1000).size();
    std::cout << sum << " "<< t1 << "\n";

    sum=0;
    t1.restart();
    for (int i=0; i<1e4; ++i)
        sum += test_a(1000).size();
    std::cout << sum << " "<< t1 << "\n";

    sum=0;
    t1.restart();
    for (int i=0; i<1e4; ++i)
        sum += test_s(1000).size();
    std::cout << sum << " "<< t1 << "\n";

    return 0;
}
