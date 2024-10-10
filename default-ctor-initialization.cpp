#include <iostream>

class A {
    public:
    A(){}
    int i;
    int j;
};

class B {
    public:
    B() = default;
    int i;
    int j;
};

struct C {
    int i;
    int j;
};

int main()
{
    for( int i = 0; i < 10; ++i) {
        A* pa = new A();
        B* pb = new B();
        C pc;
        std::cout << "A: " << pa->i << "," << pa->j << std::endl;
        std::cout << "B: " << pb->i << "," << pb->j << std::endl;
        std::cout << "C: " << pc.i << "," << pc.j << std::endl;
        delete pa;
        delete pb;
    }
  return 0;
}
