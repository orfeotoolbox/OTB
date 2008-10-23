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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
  REGISTER_TEST(otbLabelizeConnectedThresholdImageFilterNew);
  REGISTER_TEST(otbLabelizeConnectedThresholdImageFilter);
  REGISTER_TEST(otbLabelizeNeighborhoodConnectedImageFilterNew);
  REGISTER_TEST(otbLabelizeNeighborhoodConnectedImageFilter);
  REGISTER_TEST(otbLabelizeConfidenceConnectedImageFilterNew);
  REGISTER_TEST(otbLabelizeConfidenceConnectedImageFilter);
  REGISTER_TEST(otbEuclideanDistanceWithMissingValueNew);
  REGISTER_TEST(otbEuclideanDistanceWithMissingValue);
  REGISTER_TEST(otbConvolutionImageFilterNew);
  REGISTER_TEST(otbConvolutionImageFilter);
  REGISTER_TEST(otbScalarToRainbowRGBPixelFunctorNew);
  REGISTER_TEST(otbScalarToRainbowRGBPixelFunctor);
}
