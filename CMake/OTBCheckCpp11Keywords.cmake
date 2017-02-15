include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

CHECK_CXX_SOURCE_COMPILES("
#ifdef _MSC_VER
  #if _MSC_VER <= 1700
    #error Compiler is not C++11 compliant
  #endif
#else
  #if __cplusplus <= 199711L
    #error Compiler is not C++11 compliant
  #endif
#endif

int main(int argc, char *argv[])
{
  return 0;
}

"
OTB_HAS_CXX11 )


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

CHECK_CXX_SOURCE_COMPILES("
#include <cstddef>
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

