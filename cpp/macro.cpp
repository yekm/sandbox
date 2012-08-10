#include <iostream>
#include <string>

#define Q(name) #name
#define STR(n) Q(n)
#define P std::cout << std::string(" 1 "__FILE__" 2 "STR(__LINE__)" 3 ") + __func__ + "\n"

int main(int argc, const char *argv[])
{
    P;
    return 0;
}
