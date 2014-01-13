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


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbWindowedSincInterpolateImageCosineFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageCosineFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageWelchFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageWelchFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageLanczosFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageLanczosFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageBlackmanFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageBlackmanFunction);
  REGISTER_TEST(otbProlateValidationTest);
  REGISTER_TEST(otbVectorImageToAmplitudeImageFilterNew);
  REGISTER_TEST(otbVectorImageToAmplitudeImageFilter);
  REGISTER_TEST(otbVectorImageToIntensityImageFilterNew);
  REGISTER_TEST(otbVectorImageToIntensityImageFilter);
  REGISTER_TEST(otbAmplitudeFunctorTest);
  REGISTER_TEST(otbPhaseFunctorTest);
}
