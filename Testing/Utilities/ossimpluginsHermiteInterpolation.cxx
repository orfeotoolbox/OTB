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
#include <iomanip>
#include <cmath>

#include "ossim/base/ossimCommon.h"
#include "ossim/otb/HermiteInterpolator.h"
#include "itkTimeProbe.h"

int ossimpluginsHermiteInterpolationTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  double epsilon = 0.0000001;

  double xref[11];
  xref[0] = 56640.0;
  xref[1] = 56700.0;
  xref[2] = 56760.0;
  xref[3] = 56820.0;
  xref[4] = 56880.0;
  xref[5] = 56940.0;
  xref[6] = 57000.0;
  xref[7] = 57060.0;
  xref[8] = 57120.0;
  xref[9] = 57180.0;
  xref[10] = 57240.0;

  double yref[11];

  yref[0] = -1556122.3685;
  yref[1] = -1489827.1436;
  yref[2] = -1416422.8137;
  yref[3] = -1336447.1933;
  yref[4] = -1250469.0644;
  yref[5] = -1159084.8222;
  yref[6] = -1062914.9785;
  yref[7] = -962600.5575;
  yref[8] = -858799.3885;
  yref[9] = -752182.3142;
  yref[10] = -643429.3481;

  double dyref[11];

  dyref[0] = 1042.82980;
  dyref[1] = 1165.61122;
  dyref[2] = 1279.70506;
  dyref[3] = 1384.56817;
  dyref[4] = 1479.71193;
  dyref[5] = 1564.70493;
  dyref[6] = 1639.17497;
  dyref[7] = 1702.81080;
  dyref[8] = 1755.36374;
  dyref[9] = 1796.64883;
  dyref[10] = 1826.54539;


  double x = 56942.862208;

  std::cout << " *** Test y and dy ***"  << std::endl;
  ossimplugins::HermiteInterpolator* interp = new ossimplugins::HermiteInterpolator(11, xref, yref, dyref);

  double yExpected = -1154600.87561283;
  double dyExpected = 1568.49913322402;
  double y = 0;
  double dy = 0;
  interp->Interpolate(x, y, dy);


  std::cout << std::setprecision(15) << "Value at " << x << " : " << y << " (derivative " << dy << ")\n";
  std::cout << "- Should be :           " << yExpected << " (derivative " << dyExpected << ")" << std::endl;

  if ( ossim::isnan(y) || ( (y - yExpected)/yExpected  > epsilon)) return EXIT_FAILURE;
  if ( ossim::isnan(dy) ||  ( (dy - dyExpected)/dyExpected  > epsilon)) return EXIT_FAILURE;

  //Test limit situation
  x = 56640.0;
  yExpected = -1556122.3685;
  dyExpected = 1042.82980;
  interp->Interpolate(x, y, dy);
  std::cout << std::setprecision(15) << "Value at " << x << " : " << y << " (derivative " << dy << ")\n";
  std::cout << "- Should be :    " << yExpected << " (derivative " << dyExpected << ")" << std::endl;

  if ( ossim::isnan(y) || ( (y - yExpected)/yExpected  > epsilon)) return EXIT_FAILURE;
  if ( ossim::isnan(dy) || ( (dy - dyExpected)/dyExpected  > epsilon)) return EXIT_FAILURE;

  x = 56700.0;
  yExpected = -1489827.1436;
  dyExpected = 1165.61122;
  interp->Interpolate(x, y, dy);
  std::cout << std::setprecision(15) << "Value at " << x << " : " << y << " (derivative " << dy << ")\n";
  std::cout << "- Should be :    " << yExpected << " (derivative " << dyExpected << ")" << std::endl;

  if ( ossim::isnan(y) ||  ( (y - yExpected)/yExpected  > epsilon)) return EXIT_FAILURE;
  if ( ossim::isnan(dy) || ( (dy - dyExpected)/dyExpected  > epsilon)) return EXIT_FAILURE;

  x = 56942.862208;

  //Performance test
  int nTest = 1000000;
  itk::TimeProbe chrono;
  chrono.Start();
  for (int i = 0; i < nTest; ++i)
    {
    double x2 = x + 200./nTest;
    interp->Interpolate(x2, y, dy);
    }

  chrono.Stop();
  std::cout << "Computation time: " << chrono.GetMean() <<  " s" << std::endl;


  std::cout << " *** Test y only ***"  << std::endl;
  ossimplugins::HermiteInterpolator* interp2 = new ossimplugins::HermiteInterpolator(11, xref, yref, dyref);

  double y2 = 0;
  interp2->Interpolate(x, y2);

   std::cout << std::setprecision(15) << "Value at " << x << " : " << y2 << "\n";
   std::cout << "- Should be :           -1154600.87561283" << std::endl;

   //Performance test
   itk::TimeProbe chrono2;
   chrono2.Start();

   for (int i = 0; i < nTest; ++i)
     {
     double x2 = x + 200./nTest;
     interp2->Interpolate(x2, y2);
     }

   chrono2.Stop();
   std::cout << "Computation time: " << chrono2.GetMean() << " s" << std::endl;

  return EXIT_SUCCESS;
}
