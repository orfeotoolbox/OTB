include(CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("
#include <iostream>
#include <memory>

struct Foo
{
  Foo()  { }
  ~Foo() { }
};

int main(int argc, char *argv[])
{
  std::unique_ptr<Foo> p(new Foo);
  return 0;
}

"
OTB_CXX_HAS_UNIQUE_PTR )


CHECK_CXX_SOURCE_COMPILES("
#include <iostream>
struct A
{
  A() {}
  ~A() {}
  virtual void foo() { std::cout << \"A::foo()\n\"; }
};

struct B : A
{
  B() {}
  ~B() {}
  void foo() override { std::cout << \"B::foo()\n\"; }
};

int main(int argc, char *argv[])
{
  A* p = new B;
  p->foo();
  return 0;
}
"
OTB_CXX_HAS_OVERRIDE_SPECIFIER )


CHECK_CXX_SOURCE_COMPILES("
#include <iostream>
int main(int argc, char *argv[])
{
  int *p = nullptr;
  return 0;
}
"
OTB_CXX_HAS_NULLPTR )

# try_compile(
#   OTB_HAS_CXX11
#   ${CMAKE_CURRENT_BINARY_DIR}/CMake
#   ${CMAKE_SOURCE_DIR}/CMake/otbTestUniquePtr.cpp
#   )

CHECK_CXX_SOURCE_COMPILES("
#if __cplusplus <= 199711L
  #error Compiler is not C++11 compliant
#endif

int main(int argc, char *argv[])
{
  return 0;
}
"
OTB_HAS_CXX11 )
