#include <iostream>
#include <memory>

struct Foo
{
  Foo() { std::cout << "Constructor\n"; }
  ~Foo() { std::cout << "Destructor\n"; }
};

int main(int argc, char *argv[])
{
  std::unique_ptr<Foo> p(new Foo);
  return 0;
}
