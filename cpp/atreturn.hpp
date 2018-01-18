#include <functional>

class AtReturn
{
public:
    AtReturn(std::function<void(void)> f)
        : m_f(f)
    {}
    ~AtReturn()
    {
        m_f();
    }
private:
    std::function<void(void)> m_f;
};
