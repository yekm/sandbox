#ifndef NUMERALSYSTEM_H
#define NUMERALSYSTEM_H

#include <iostream>
#include <vector>
#include <algorithm>

template<int N, typename T = unsigned int>
class NumeralSystem
{
public:
    explicit NumeralSystem(T num)
        : m_numbers()
    {
        if (num == 0)
        {
            m_numbers.push_back(0);
            return;
        }

        while (num != 0)
        {
            m_numbers.push_back(num % N);
            num = num / N;
        }
    }
    std::string to_str() const
    {
        std::string res;
        res.resize(m_numbers.size());
        std::transform(m_numbers.rbegin(), m_numbers.rend(), res.begin(), MapNumber<T>());
        return res;
    }
private:
    std::vector<T> m_numbers;

    template <int FN, typename FT>
    friend std::ostream & operator<< (std::ostream &, const NumeralSystem<FN, FT>&);

    template<typename C = int>
    class MapNumber
    {
    public:
        explicit MapNumber()
            : m_number_map("0123456789abcdefghijklmnopqrstuзабыл")
        {}
        char operator()(const C & number)
        {
            return m_number_map[number];
        }
    private:

        const std::string m_number_map;
    };
};


template<int N, typename T>
std::ostream & operator<<(std::ostream &os, const NumeralSystem<N, T>& ns)
{
    return os << ns.to_str();
}

#endif /* NUMERALSYSTEM_H */
