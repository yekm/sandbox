// https://www.reddit.com/r/cpp/comments/6nz79f/complex_arithmetic_is_complicated_the_search_for/
// https://medium.com/@smcallis_71148/complex-arithmetic-is-complicated-873ec0c69fc5

#include <iostream>
#include <complex>
#include <algorithm>
#include <vector>
#include <numeric>

typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;
pcg32_random_t rng_{123, 456};
pcg32_random_t * rng = &rng_;

uint32_t pcg32_random_r()
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

template <typename T>
T complexrand()
{
    T c(
    0xffffffff/((float)pcg32_random_r() + 1),
    0xffffffff/((float)pcg32_random_r() + 1)
            );
    return c;
}

// complex numbers
//typedef _Complex float      c99float;
typedef std::complex<float> stlfloat;

struct mycfloat {
    mycfloat()                                    {}
    mycfloat(float re, float im) : re(re), im(im) {}
    float re,im;
};

// operators
mycfloat operator+(const mycfloat &a, const mycfloat &b) {
    return mycfloat(a.re + b.re, a.im + b.im);
}

mycfloat operator*(const mycfloat &a, const mycfloat &b) {
    return mycfloat(a.re*b.re - a.im*b.im, a.im*b.re + a.re*b.im);
}

std::ostream& operator<< (std::ostream& stream, const mycfloat &c)
{
    stream << c.re << ", " << c.im;
    return stream;
}

template <typename T>
T b(size_t n)
{
    std::vector<T> a(n), b(n), c(n), d(n);
    T u, v;

    std::generate(a.begin(), a.end(), complexrand<T>);
    std::generate(b.begin(), b.end(), complexrand<T>);
    std::generate(c.begin(), c.end(), complexrand<T>);

    u = complexrand<T>();
    v = complexrand<T>();

    auto aa = a.begin();
    auto bb = b.begin();
    auto cc = c.begin();
    auto dd = d.begin();
    while (dd != d.end())
    {
        *dd++ = (*aa++)*u + (*bb++)*v + (*cc++);
    }
    return std::accumulate(d.begin(), d.end(), T{});
}

int main(int argc, char *argv[])
{
    /* g++ -std=c++17 -Wall -Wextra -O3 -fcx-limited-range complexbench.cpp
     * time ./a.out $((1024*1024*64))
     */

    //auto ret = b<stlfloat>(std::stoi(argv[1]));
    auto ret = b<mycfloat>(std::stoi(argv[1]));
    std::cout << ret << std::endl;
    return 0;
}
