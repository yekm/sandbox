#include "caller.h"
#include <boost/bind.hpp>
#include <limits>

namespace common {

Caller::Caller()
    : m_work(new boost::asio::io_service::work(m_io))
    , m_loop(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &m_io)
    , m_last_caller(0)
{
}

Caller::~Caller()
{
    m_work.reset(); // allow io to stop
    m_io.stop(); // stop processing
    m_loop.join(); // join io::run
}

Caller::CallerHandler Caller::register_callback(CallbackType callback, unsigned int period)
{
    if (m_last_caller == std::numeric_limits<CallerHandler>::max())
        throw std::runtime_error("y u no stop?");

    CallerHandler new_id;
    if(m_deleted_handlers.empty())
    {
        new_id = m_last_caller++;
    }
    else
    {
        new_id = m_deleted_handlers.back();
        m_deleted_handlers.pop_back();
    }

    //m_callbacks.emplace(new_id, std::unique_ptr<PeriodicCaller>(new PeriodicCaller(m_io, callback, period)));
    m_callbacks.insert(std::make_pair(new_id, std::unique_ptr<PeriodicCaller>(new PeriodicCaller(m_io, callback, period))));
    return new_id;
}

void Caller::modify_period(CallerHandler id, unsigned int new_period)
{
    if (m_callbacks.find(id) == m_callbacks.end())
        throw std::runtime_error("no such callback id");
    m_callbacks[id].get()->set_period(new_period);
}

void Caller::delete_caller(CallerHandler id)
{
    if (m_callbacks.find(id) == m_callbacks.end())
        throw std::runtime_error("no such callback id");
    m_callbacks.erase(id);
    m_deleted_handlers.push_back(id);
}

Caller::PeriodicCaller::PeriodicCaller(boost::asio::io_service & io, CallbackType c, unsigned int p)
    : m_callback(c)
    , m_period(p)
    , m_timer(io, boost::posix_time::seconds(p))
{
    rewait();
}

void Caller::PeriodicCaller::set_period(unsigned int new_period)
{
    m_period = new_period;
    m_timer.expires_from_now(boost::posix_time::seconds(m_period));
    rewait();
}

void Caller::PeriodicCaller::rewait()
{
    m_timer.async_wait(boost::bind(&Caller::PeriodicCaller::tick, this, boost::asio::placeholders::error));
}

void Caller::PeriodicCaller::tick(const boost::system::error_code& error)
{
    if (!error)
    {
        m_callback();
        m_timer.expires_at(m_timer.expires_at() + boost::posix_time::seconds(m_period));
        rewait();
    }
    else
    {
        //std::warn << "Timer with period " << m_period << ", error: " << error.message();
    }
}

} // namespace common
