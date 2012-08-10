#include <iostream>
using namespace std;

int main() {
        cout <<
                "char\t" << sizeof(char) << endl <<
                "short\t" << sizeof(short) << endl <<
                "int\t" << sizeof(int) << endl <<
                "int*\t" << sizeof(int*) << endl <<
                "l\t" << sizeof(long) << endl <<
                "long[1024*256]\t" << sizeof(long[1024*256]) << endl <<
                "l l\t" << sizeof(long long) << endl <<
                "float\t" << sizeof(float) << endl <<
                "double\t" << sizeof(double) << endl;
        return 0;
}
