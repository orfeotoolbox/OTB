#include <iostream>

struct A
{
  A() {}
  ~A() {}
  virtual void foo() { std::cout << "A::foo()\n"; }
};

struct B : A
{
  B() {}
  ~B() {}
  void foo() override { std::cout << "B::foo()\n"; }
};

int main(int argc, char *argv[])
{
  A* p = new B;
  p->foo();
  return 0;
}
