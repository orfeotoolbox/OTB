include(CheckCXXSourceCompiles)

unset(OTB_HAS_CXX11 CACHE)
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

unset(OTB_CXX_HAS_UNIQUE_PTR CACHE)
CHECK_CXX_SOURCE_COMPILES("
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

unset(OTB_CXX_HAS_OVERRIDE_SPECIFIER CACHE)
CHECK_CXX_SOURCE_COMPILES("
struct A
{
  A()  { }
  ~A() { }
  virtual void foo( ) {  }
};

struct B : A
{
  B()  { }
  ~B() { }
  void foo( ) override {  }
};

int main(int argc, char *argv[])
{
  A* p = new B;
  p->foo();
  return 0;
}

"
OTB_CXX_HAS_OVERRIDE_SPECIFIER )

unset(OTB_CXX_HAS_NULLPTR CACHE)
CHECK_CXX_SOURCE_COMPILES("
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

