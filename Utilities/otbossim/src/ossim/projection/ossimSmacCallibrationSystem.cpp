//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Proecedure for compensation of aerial camera lens distortion
// as computed by the Simultaneous Multiframe Analytical Calibration
// (SMAC) System:
// 
//*******************************************************************
//  $Id$
#include <ossim/projection/ossimSmacCallibrationSystem.h>
#include <cmath>
#include <iostream>

void ossimSmacCallibrationSystem::undistort(const ossimDpt& input, ossimDpt& output)const
{
   double x = input.x - theCenter.x;
   double y = input.y - theCenter.y;
   double x2 = x*x;
   double y2 = y*y;
   double r = std::sqrt(x2 + y2);
   double r2 = r*r;
   double r4 = r2*r2;
   double r6 = r2*r4;
   double r8 = r4*r4;
   double sum = (_symmetricRadialDistortionCoefficients[0] +
                 _symmetricRadialDistortionCoefficients[1]*r2 +
                 _symmetricRadialDistortionCoefficients[2]*r4 +
                 _symmetricRadialDistortionCoefficients[3]*r6 +
                 _symmetricRadialDistortionCoefficients[4]*r8);
   double DXr = x*(sum);
   double DYr = y*(sum);
   double DXd = ((1 + _decenteringDistortionCoefficients[2]*r2 + _decenteringDistortionCoefficients[3]*r4)*
                 (_decenteringDistortionCoefficients[0]*(r2 + 2*x*x) + 2*_decenteringDistortionCoefficients[1]*x*y));
   double DYd = ((1 + _decenteringDistortionCoefficients[2]*r2 + _decenteringDistortionCoefficients[3]*r4)*
                 (2*_decenteringDistortionCoefficients[0]*x*y + _decenteringDistortionCoefficients[1]*(r2 + 2*y*y)));
   
   output.x = x + DXr + DXd;
   output.y = y + DYr + DYd;
}

void ossimSmacCallibrationSystem::test()
{
   // this example was grabbed from a sample callibration report;
   double k0 = -.2165e-3;
   double k1 = .4230e-7;
   double k2 = -.1652e-11;
   double k3 = .2860e-19;
   double k4 = .5690e-26;
   double p1 = -.1483e-6;
   double p2 = .1558e-6;
   double p3 = -.1464e-18;
   double p4 = .1233e-38;

   ossimSmacCallibrationSystem smac(k0, k1, k2, k3, k4,
                                    p1, p2, p3, p4,
                                    ossimDpt(.003, -.001));
   ossimDpt output;
   smac.undistort(ossimDpt(62.142, -62.336), output);
   std::cout << "Input: " << ossimDpt(62.142, -62.336) << std::endl;
   std::cout << "Output should be close to x = 62.136 and y = -62.332 <====> " << output << std::endl;
   ossimDpt input = output;
   std::cout << "Now test the inverse!" << std::endl;
   smac.distort(input, output);
   
   std::cout << "Inverse should be close to " << ossimDpt(62.142, -62.336) << " <=====> " << output << std::endl;
}

