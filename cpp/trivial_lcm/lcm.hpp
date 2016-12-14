#ifndef LCM_HPP
#define LCM_HPP

#include <map>

template <typename T>
class LCM
{
public:
    void add(T x)
    {
        //T x2 = x;
        std::map<T, T> factors;

        T prime_limit = std::sqrt(x) + 1;

        for (T prime=2; prime<=prime_limit; ++prime)
        {
            if (is_prime(prime))
            {
                while (x % prime == 0)
                {
                    factors[prime] += 1;
                    x /= prime;
                }
            }
        }

        if (x > 1)
            factors[x]++;

        LCM<T>::join_factors(m_factors, factors);
    }

    const std::map<T, T> & get_factors() const
    {
        return m_factors;
    }

    static void join_factors(std::map<T, T> & m1, const std::map<T, T> & m2)
    {
        for (const auto & f2 : m2)
            m1[f2.first] = std::max(m1[f2.first], f2.second);
    }

private:
    bool is_prime(T n)
    {
        if (n <= 3)
            return n > 1;

        if (n % 2 == 0 || n % 3 == 0)
            return false;

        T max_i = sqrt(n);
        for (T i = 5; i <= max_i; i += 6)
        {
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        }

        return true;
    }

    std::map<T, T> m_factors;
};

#endif // LCM_HPP
