
#include <iostream>
#include <fstream>

#include "imaging/ErsSar/ErsSarLeader/ErsSarLeader.h"
// #include "AlosPalsarLeader.h"

int main(int argc, char * argv[])
{
  std::ifstream ifs(argv[1], std::ifstream::in);
  ErsSarLeader leader;
//   AlosPalsarLeader leader;
  ifs >> leader;
  ifs.close();
  std::cout << leader;
  

}
