#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "lcm.hpp"

int main(int, char**)
{
    //utils::Debugging::get().set_verbosity(utils::Debugging::V_DEBUG);

    typedef uint64_t prime_type;

    const int num_threads = 10;
    std::vector<std::thread> pool;
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<prime_type> queue;
    bool done = false;
    std::vector<LCM<prime_type>> lcm(num_threads);

    for (int i=0; i<num_threads; ++i)
    {
        pool.push_back(std::thread([&,i](){
            while(true)
            {
                std::unique_lock<std::mutex> lk(mutex);
                if (!done)
                    cv.wait(lk, [&]{return queue.size() != 0 || done;});
                if (done && queue.size() == 0)
                    return;
                prime_type p = queue.front();
                queue.pop();
                lk.unlock();
                lcm.at(i).add(p);
            }
        }));
    }

    prime_type x;
    while (std::cin >> x)
    {
        {
            std::lock_guard<std::mutex> lk(mutex);
            queue.push(x);
        }
        cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lk(mutex);
        done = true;
    }
    cv.notify_all();
    for (auto & t : pool)
    {
        t.join();
    }

    std::map<prime_type, prime_type> factors;
    for (auto & l : lcm)
    {
        LCM<prime_type>::join_factors(factors, l.get_factors());
    }

    for (auto & f : factors)
    {
        std::cout << f.first << "^" << f.second << " * ";
    }
    std::cout << "1\n";
    return 0;
}
