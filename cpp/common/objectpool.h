#ifndef _OBJECTPOOL_H_
#define _OBJECTPOOL_H_

#include <deque>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>

template<typename T>
class ObjectPool
{
public:

    typedef std::unique_ptr<T> pointer_type;

    class Boomerang
    {
    public:
        typedef std::function< void(pointer_type) > against_function_type;

        Boomerang() = delete;
        Boomerang(const Boomerang&) = delete;
        Boomerang(pointer_type &&pointer, against_function_type against)
            : m_pointer(std::move(pointer))
            , m_against(against)
        {
        }
        Boomerang(Boomerang &&) = default;
        Boomerang& operator= (Boomerang &&) = delete;

        T* operator->()
        {
            return m_pointer.get();
        }

        ~Boomerang()
        {
            if (m_pointer)
                m_against(std::move(m_pointer));
        }
    private:
        pointer_type m_pointer;
        against_function_type m_against;
    };

    typedef std::function< pointer_type(void) > create_function_type;

    ObjectPool(create_function_type create, unsigned int initial_size = 0)
        : m_create(create)
        , m_initial_size(initial_size)
    {
        while (initial_size > 0)
        {
            m_pool.push_back(m_create());
            --initial_size;
        }
    }

    virtual ~ObjectPool()
    {
    }

    Boomerang get()
    {
        std::lock_guard<std::mutex> l(m_mutex);
        if (m_pool.size() > 0)
        {
            pointer_type temp(std::move(m_pool.front()));
            m_pool.pop_front();
            return Boomerang( std::move(temp), std::bind(&ObjectPool::put, this, std::placeholders::_1) );
        }
        else
        {
            return Boomerang( pointer_type(m_create()), std::bind(&ObjectPool::put, this, std::placeholders::_1) );
        }
    }

    virtual void put(pointer_type && p)
    {
        std::lock_guard<std::mutex> l(m_mutex);
        if (m_pool.size() < m_initial_size)
            m_pool.push_front(std::move(p));
    }

protected:
    std::mutex m_mutex;
    std::deque< pointer_type > m_pool;

private:
    create_function_type m_create;
    unsigned int m_initial_size;
};

template<typename T>
class ObjectPoolDrainless : public ObjectPool<T>
{
public:
    typedef ObjectPool<T> base;
    ObjectPoolDrainless(typename base::create_function_type create, unsigned int initial_size = 0)
        : ObjectPool<T>(create, initial_size)
    {}
    virtual void put(typename base::pointer_type && p)
    {
        std::lock_guard<std::mutex> l(base::m_mutex);
        base::m_pool.push_front(std::move(p));
    }
};

#endif // _OBJECTPOOL_H_
