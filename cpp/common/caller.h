#ifndef CALLER_H
#define CALLER_H

#include <vector>
#include <memory>
#include <thread>
#include <boost/asio.hpp>

namespace common {

class Caller
{
public:
    typedef std::function<void(void)> CallbackType;
    typedef unsigned int CallerHandler;

    Caller();
    ~Caller();
    CallerHandler register_callback(CallbackType callback, unsigned int period);
    void modify_period(CallerHandler id, unsigned int new_period);
    void delete_caller(CallerHandler id);
    Caller(const Caller&) = delete;
    Caller& operator=(const Caller&) = delete;
private:
    class PeriodicCaller
    {
    public:
        PeriodicCaller(boost::asio::io_service & io, CallbackType c, unsigned int p);
        void set_period(unsigned int new_period);
        PeriodicCaller(const PeriodicCaller&) = delete;
        PeriodicCaller& operator=(const PeriodicCaller&) = delete;
    private:
        CallbackType m_callback;
        unsigned int m_period;
        boost::asio::deadline_timer m_timer;
        void rewait();
        void tick(const boost::system::error_code& error);
    };
    boost::asio::io_service m_io;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::thread m_loop;
    std::map<CallerHandler, std::unique_ptr<PeriodicCaller>> m_callbacks;
    CallerHandler m_last_caller;
    std::vector<CallerHandler> m_deleted_handlers;
};

} // namespace common

#endif // CALLER_H
