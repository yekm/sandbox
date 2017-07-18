#include <iostream>
#include <unordered_map>

// from <linux/hash.h>
uint64_t hash_64(uint64_t val, unsigned int bits)
{
	uint64_t hash = val;

	/*  Sigh, gcc can't optimise this alone like it does for 32 bits. */
	uint64_t n = hash;
	n <<= 18;
	hash -= n;
	n <<= 33;
	hash -= n;
	n <<= 3;
	hash += n;
	n <<= 3;
	hash -= n;
	n <<= 4;
	hash += n;
	n <<= 2;
	hash += n;

	/* High bits are more random, so use them. */
	return hash >> (64 - bits);
}

// from http://www.pcg-random.org/download.html
typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;

uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

int main(int argc, const char *argv[])
{
    /*
    uint64_t bits, from, to;
    bits = std::stoi(argv[1]);
    from = std::stoi(argv[2]);
    to = std::stoi(argv[3]);
      */

    pcg32_random_t seed;

    std::cout << "number of elements\thash bits\tcollisions" << std::endl;
    for (auto bits = 1; bits < 32; ++bits)
    {
        for (auto i = 1; i < 1024*64; i *= 2)
        {
            std::unordered_map<uint64_t, uint64_t> m;
            for (auto ii = 1; ii < i; ++ii)
            {
                //m[hash_64(ii, bits)]++;
                m[hash_64(pcg32_random_r(&seed), bits)]++;
            }
            uint64_t sum = 0;
            for (auto kv : m)
            {
                if (kv.second > 1)
                    sum += kv.second;
            }
            std::cout << i << "\t" << bits << "\t" << sum << std::endl;
        }
        std::cout << std::endl;

    }

    // g++ -Ofast -march=native -std=c++17 hashtest.cpp && ./a.out | pv >hashtest
    // gnuplot -p -e "set logscale cb 2; plot 'hashtest' using 1:2:3 with image"

    return 0;
}
