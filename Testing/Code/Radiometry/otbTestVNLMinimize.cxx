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

#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>
#include <iostream>

int otbTestVNLMinimize(int argc, char * argv[])
{
	std::cout << "-------------------------- Start with LINUX coeff: "<< std::endl;

	vnl_sparse_matrix<double> a(5, 6);
    vnl_vector<double> b(5), bestParams(6);
    b.fill(0);
    bestParams.fill(0);


a(0,0) = 1;
a(0,1) = 0.500098;
a(0,2) = 0.5;
a(0,3) = 0.250049;
a(0,4) = 0.25;
a(0,5) = 0.125024;

a(1,0) = 1;
a(1,1) = 1;
a(1,2) = 7.93399e-05;
a(1,3) = 7.93399e-05;
a(1,4) = 6.29482e-09;
a(1,5) = 6.29482e-09;

a(2,0) = 1;
a(2,1) = 1;
a(2,2) = 0.987544;
a(2,3) = 0.987544;
a(2,4) = 0.975242;
a(2,5) = 0.975242;

a(3,0) = 1;
a(3,1) = 9.75229e-05;
a(3,2) = 7.93399e-05;
a(3,3) = 7.73746e-09;
a(3,4) = 6.29482e-09;
a(3,5) = 6.13889e-13;

a(4,0) = 1;
a(4,1) = 9.75229e-05;
a(4,2) = 0.987226;
a(4,3) = 9.62772e-05;
a(4,4) = 0.974616;
a(4,5) = 9.50474e-05;

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

std::cout << "-------------------------- Start with visual coeff: "<< std::endl;

	vnl_sparse_matrix<double> a2(5, 6);
    vnl_vector<double> b2(5), bestParams2(6);
    b2.fill(0);
    bestParams2.fill(0);

a2(0,0) = 1;
a2(0,1) = 0.500098;
a2(0,2) = 6302;
a2(0,3) = 3151.61;
a2(0,4) = 3.97152e+007;
a2(0,5) = 1.98615e+007;

a2(1,0) = 1;
a2(1,1) = 1;
a2(1,2) = 1.00008;
a2(1,3) = 1.00008;
a2(1,4) = 1.00016;
a2(1,5) = 1.00016;

a2(2,0) = 1;
a2(2,1) = 1;
a2(2,2) = 12447;
a2(2,3) = 12447;
a2(2,4) = 1.54928e+008;
a2(2,5) = 1.54928e+008;

a2(3,0) = 1;
a2(3,1) = 9.75229e-005;
a2(3,2) = 1.00008;
a2(3,3) = 9.75307e-005;
a2(3,4) = 1.00016;
a2(3,5) = 9.75384e-005;
a2(4,0) = 1;
a2(4,1) = 9.75229e-005;
a2(4,2) = 12443;
a2(4,3) = 1.21348;
a2(4,4) = 1.54828e+008;
a2(4,5) = 15099.3;

b2(0) = 0.451568;
b2(1) = 0.440229;
b2(2) = 0.462236;
b2(3) = 0.44059;
b2(4) = 0.462275;

vnl_sparse_matrix_linear_system<double> linearSystem2(a2, b2);
std::cout << "Satart solve init:" << std::endl;
    // And solve it
    vnl_lsqr linearSystemSolver2(linearSystem2);
	std::cout << "Solve init done: " <<bestParams2<< std::endl;
    linearSystemSolver2.minimize(bestParams2);
	std::cout << "Solve done: " <<bestParams2<< std::endl;

	return EXIT_SUCCESS;
}