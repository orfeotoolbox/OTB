
#include <cstdlib>
#include <iostream>

int test_clean_main( int argc, char* argv[] )
{
  int i = system("/usr/bin/make -C ../../../../../.. clean");
  std::cout << "Return value: " << i << std::endl;
  return EXIT_SUCCESS;
}
