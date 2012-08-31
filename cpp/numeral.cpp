#include <iostream>
#include "common/numeralsystem.h"

int main(int /*argc*/, const char *argv[])
{
    std::cout << NumeralSystem<16>(atoi(argv[1])) << " 16\n"
              << NumeralSystem<10>(atoi(argv[1])) << " 10\n"
              << NumeralSystem<8>(atoi(argv[1])) << " 8\n"
              << NumeralSystem<2>(atoi(argv[1])) << " 2\n"
              << NumeralSystem<3>(atoi(argv[1])) << " 3\n"
              ;
    return 0;
}
