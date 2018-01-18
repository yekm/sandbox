#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <mutex>
#include <thread>
#include <deque>
#include <functional>

template <typename T>
class ObjectPool
{
public:

    typedef std::unique_ptr<T, std::function<void(T*)>> returned_pointer_type;
    //typedef std::unique_ptr<T, decltype(&this->put)> pointer_type;

    /*
    template <typename... Args>
    ObjectPool(std::function<T*(Args)> create_func, Args&&... args);
    {
        m_create = std::bind(create_func, std::forward<Args>(args...));
    }
    */

    typedef std::function< T*(void) > create_function_type;

    ObjectPool(create_function_type cft)
        : m_create(cft)
    {}

    returned_pointer_type get()
    {
        std::lock_guard<std::mutex> l(m_mutex);
        if (m_pool.empty())
            return returned_pointer_type(m_create(),
                                         [&](T* ptr) { put(ptr); } );
        else
        {
            returned_pointer_type p(std::move(m_pool.front()),
                                    [&](T* ptr) { put(ptr); } );
            m_pool.pop_front();
            return std::move(p);
        }
    }

    void put(T* object)
    {
        std::lock_guard<std::mutex> l(m_mutex);
        m_pool.push_front(object);
    }

private:
    std::mutex m_mutex;
    std::deque<T*> m_pool;
    create_function_type m_create;
};

#endif // OBJECTPOOL_HPP
