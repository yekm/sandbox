#include <iostream>

class Base
{
public:
    Base(int x_) : x(x_) { std::cout << "Base " << x << "\n"; }
    ~Base() { std::cout << "~Base " << x << "\n"; }
private:
    int x;
};

class A1 : virtual public Base
{
public:
    A1(int x_) : Base(x_), x(x_) { std::cout << "A1 " << x << "\n"; }
    ~A1() { std::cout << "~A1 " << x << "\n"; }
private:
    int x;
};

class A2 : virtual public Base
{
public:
    A2(int x_) : Base(x_), x(x_) { std::cout << "A2 " << x << "\n"; }
    ~A2() { std::cout << "~A2 " << x << "\n"; }
private:
    int x;
};

class B : public A1, public A2
{
public:
    B(int x_) : A1(1), A2(2), x(x_), Base(5) { std::cout << "B " << x << "\n"; }
    ~B() { std::cout << "~B " << x << "\n"; }
private:
    int x;
};


class P
{
public:
    P(int x_) : x(x_) { std::cout << "P " << x << "\n"; }
    virtual ~P() { std::cout << "~P " << x << "\n"; }
private:
    int x;
};

class D : public P
{
public:
    D(int x_) : x(x_), P(x_) { std::cout << "D " << x << "\n"; }
    ~D() { std::cout << "~D " << x << "\n"; }
private:
    int x;

};


int main(int argc, const char *argv[])
{
    B b(5);
    P *p1 = new P(1);
    P *p2 = new D(2);
    //*p1 = *p2;
    delete p1;
    delete p2;
    return 0;
}

