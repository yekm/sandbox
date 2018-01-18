#include <functional>

class atreturn
{
public:
    atreturn(std::function<void(void)> f)
        : m_f(f)
    {}
    ~atreturn()
    {
        m_f();
    }
private:
    std::function<void(void)> m_f;
};
