#include <iostream>
#include <unordered_map>
#include "common/numeralsystem.h"
//#include "common/dbg.h"

enum Language
{
    L_RUS,
    L_ENG,
};

template<int LANG>
class NumbersMap
{
public:
    NumbersMap(int pos)
        : m_pos(pos)
        , m_others(0)
        , m_ten(false)
    {}
    std::string n0_9(int n);
    std::string n10_90(int n);
    std::string n100_900(int n);
    std::string n1000(int n);
    std::string nothing();
private:
    int m_pos, m_others;
    bool m_ten;
    static std::unordered_map<int, std::string> m_map;
    static std::unordered_map<int, std::string> m_map0_9;
    static std::unordered_map<int, std::string> m_map10_19;
    static std::unordered_map<int, std::string> m_map20_90;
    static std::unordered_map<int, std::string> m_map100_900;
    static std::unordered_map<int, std::string> m_alt_map;
    static std::unordered_map<int, std::string> m_1;
    static std::unordered_map<int, std::string> m_2;
    static std::unordered_map<int, std::string> m_5;
};

template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_5 = {
    {1, "тысяч "},
    {2, "миллионов "},
    {3, "миллиардов "}
};

template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_2 = {
    {1, "тысячи "},
    {2, "миллиона "},
    {3, "миллиарда "}
};

template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_1 = {
    {1, "тысяча "},
    {2, "миллион "},
    {3, "миллиард "}
};

template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_alt_map = {
    {1, "одна "},
    {2, "две "}
};

template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_map0_9 = {
//    {0, "ноль "},
    {1, "один "},
    {2, "два "},
    {3, "три "},
    {4, "четыре "},
    {5, "пять "},
    {6, "шесть "},
    {7, "семь "},
    {8, "восемь "},
    {9, "девять "},
};
template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_map10_19 = {
    {0, "десять "},
    {1, "одинадцать "},
    {2, "двенадцать "},
    {3, "тринажцать "},
    {4, "четырнадцать "},
    {5, "пятьнадцать "},
    {6, "шестьнадцать "},
    {7, "семьнадцать "},
    {8, "восемьнадцать "},
    {9, "девятьнадцать "},
};
template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_map20_90 = {
    {2, "двадцать "},
    {3, "тридцать "},
    {4, "сорок "},
    {5, "пятьдесят "},
    {6, "шестьдесят "},
    {7, "семьдесят "},
    {8, "восемьдесят "},
    {9, "девяносто "},
};
template<>
std::unordered_map<int, std::string> NumbersMap<L_RUS>::m_map100_900 = {
    {1, "сто "},
    {2, "двести "},
    {3, "триста "},
    {4, "четыреста "},
    {5, "пятьсот "},
    {6, "шестьсот "},
    {7, "семьсот "},
    {8, "восемьсот "},
    {9, "десятьсот "}
};

template<>
std::string NumbersMap<L_RUS>::n0_9(int n)
{
    if (m_ten)  // от 10 до 19 одним словом
        return m_map10_19[n];
    if (m_pos == 1 && (n == 1 || n == 2)) // исключение 1 тысяча, 2 тысячи
        return m_alt_map[n];
    return m_map0_9[n];
}

template<>
std::string NumbersMap<L_RUS>::n10_90(int n)
{
    if (n == 1) // от 10 до 19 одним словом
    {
        m_ten = true;
        return "";
    }
    return m_map20_90[n];
}

template<>
std::string NumbersMap<L_RUS>::n100_900(int n)
{
    return m_map100_900[n];
}

template<>
std::string NumbersMap<L_RUS>::n1000(int n_all)
{
    if (m_pos == 0 || n_all == 0)    // если сотня первая или в ней все числа равны 0, ничего не читается
        return "";
    switch(n_all % 10)
    {
    case 1:
        return m_1[m_pos];  // тысяча
    case 2 ... 4:
        return m_2[m_pos];  // тысячи
    case 0:
    case 5 ... 9:
        return m_5[m_pos];  // тысяч
    }
}

template<>
std::string NumbersMap<L_RUS>::nothing()
{
    return "ноль";
}



/* единственная оптимизация для англиского языка -- не переводить сотни
template<>
std::string NumbersMap<L_ENG>::n100_900(int n)
{
    if (n == 0)
        return "";
    return m_map0_9[n] + "hundred ";
}
*/


template<int LANG>
class NumReader
{
public:
    template<class ForwardIterator>
    NumReader(ForwardIterator begin, ForwardIterator end)
    {
        int third = 0;
        while ( begin != end )
        {
            std::string r;
            auto nm = NumbersMap<LANG>(third);
            int n1 = 0, n2 = 0, n3 = 0;
            n1 = *begin++;
            if (begin != end)
                n2 = *begin++;
            if (begin != end)
                n3 = *begin++;

            //D() << "processing" << n1 << n2 << n3;

            r += nm.n100_900(n3);
            r += nm.n10_90(n2);
            r += nm.n0_9(n1);
            r += nm.n1000(n1 + n2*10 + n3*100);

            if (begin == end && n1 == 0 && n2 == 0 && n3 == 0 && third == 0)
                r = nm.nothing();

            m_r = r + m_r;
            ++third;
        }
    }
    std::string to_str()
    {
        return m_r;
    }
private:
    std::string m_r;
};


int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "must provide one argument -- the number\n";
        return -1;
    }
    auto ns10 = NumeralSystem<10>(atoi(argv[1]));
    auto ns8 = NumeralSystem<8>(atoi(argv[1]));
    auto nr10 = NumReader<L_RUS>(ns10.begin(), ns10.end());
    auto nr8 = NumReader<L_RUS>(ns8.begin(), ns8.end());
    std::cout << ns10 << "(10) " << nr10.to_str() << std::endl;
    std::cout << ns8 << "(8) " << nr8.to_str() << std::endl;
    return 0;
}
