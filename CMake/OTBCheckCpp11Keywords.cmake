#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# DEPRECATED: Not used in OTB cmake configuration
# DEPRECATED: should be removed after 6.2 release

include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

unset(OTB_HAS_CXX11 CACHE)

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

