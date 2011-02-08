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
  REGISTER_TEST(otbBandMathImageFilterNew);
  REGISTER_TEST(otbBandMathImageFilter);
  REGISTER_TEST(otbBandMathImageFilterWithIdx);
  REGISTER_TEST(otbComplexToIntensityFilterTest);
  REGISTER_TEST(otbRealAndImaginaryImageToComplexImageFilterTest);
  REGISTER_TEST(otbRealImageToComplexImageFilterTest);
  REGISTER_TEST(otbImaginaryImageToComplexImageFilterTest);
  REGISTER_TEST(otbComplexToVectorImageCastFilterNew);
  REGISTER_TEST(otbComplexToVectorImageCastFilterTest);
  REGISTER_TEST(otbLog10ThresholdedImageFilterTest);
}
