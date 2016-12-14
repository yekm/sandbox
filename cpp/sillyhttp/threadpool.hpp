#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadPool
{
public:
    ThreadPool(size_t size, const std::function<void(T&&)> & func)
        : m_done(false)
    {
        while (size--)
        {
            m_pool.push_back(std::thread([&,func]()
            {
                while (true) // or while(!m_done) but actually useless
                {
                    std::unique_lock<std::mutex> lk(m_mutex);
                    /* maybe here too
                    * if (m_done)
                    *     return;
                    * */
                    m_cv.wait(lk, [&]{return m_queue.size() != 0 || m_done;});
                    if (m_done)
                        return;
                    T work(std::move(m_queue.front()));
                    m_queue.pop();
                    lk.unlock();
                    func(std::move(work));
                }
            }
            ));
        }
    }

    void submit(T && work)
    {
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            m_queue.push(std::move(work));
        }
        m_cv.notify_one();
    }

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            m_done = true;
        }
        m_cv.notify_all();
        for (auto & t : m_pool)
        {
            t.join();
        }
    }

private:
    std::queue<T> m_queue;
    std::vector<std::thread> m_pool;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_done;
};

#endif // THREADPOOL_HPP
