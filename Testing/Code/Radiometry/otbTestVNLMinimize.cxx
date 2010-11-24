/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <cstdlib>
#include <iostream>

#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>

int otbTestVNLMinimize(int argc, char * argv[])
{
  vnl_sparse_matrix<double> a(5, 6);
  vnl_vector<double> b(5), bestParams(6);
  b.fill(0);
  bestParams.fill(0);
  
  a(0,0) = 1;
  a(0,1) = 0.500098;
  a(0,2) = 6302;
  a(0,3) = 3151.61;
  a(0,4) = 3.97152e+007;
  a(0,5) = 1.98615e+007;
  
  a(1,0) = 1;
  a(1,1) = 1;
  a(1,2) = 1.00008;
  a(1,3) = 1.00008;
  a(1,4) = 1.00016;
  a(1,5) = 1.00016;

  a(2,0) = 1;
  a(2,1) = 1;
  a(2,2) = 12447;
  a(2,3) = 12447;
  a(2,4) = 1.54928e+008;
  a(2,5) = 1.54928e+008;
  
  a(3,0) = 1;
  a(3,1) = 9.75229e-005;
  a(3,2) = 1.00008;
  a(3,3) = 9.75307e-005;
  a(3,4) = 1.00016;
  a(3,5) = 9.75384e-005;
  a(4,0) = 1;
  a(4,1) = 9.75229e-005;
  a(4,2) = 12443;
  a(4,3) = 1.21348;
  a(4,4) = 1.54828e+008;
  a(4,5) = 15099.3;
  
  b(0) = 0.451568;
  b(1) = 0.440229;
  b(2) = 0.462236;
  b(3) = 0.44059;
  b(4) = 0.462275;
  
  vnl_sparse_matrix_linear_system<double> linearSystem(a, b);
  std::cout << "Satart solve init:" << std::endl;
  // And solve it
  vnl_lsqr linearSystemSolver(linearSystem);
  std::cout << "Solve init done: " <<bestParams<< std::endl;
  linearSystemSolver.minimize(bestParams);
  std::cout << "Solve done: " <<bestParams<< std::endl;

  return EXIT_SUCCESS;
}
