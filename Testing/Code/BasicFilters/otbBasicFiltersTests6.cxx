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


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbBSplineDecompositionImageFilterNew);
  REGISTER_TEST(otbBSplineDecompositionImageFilter);
  REGISTER_TEST(otbBSplineInterpolateImageFunctionNew);
  REGISTER_TEST(otbBSplineInterpolateImageFunction);
  REGISTER_TEST(otbProlateInterpolateImageFunctionNew);
  REGISTER_TEST(otbProlateInterpolateImageFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageFunctionBaseNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageGaussianFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageGaussianFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageHammingFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageHammingFunction);
}
