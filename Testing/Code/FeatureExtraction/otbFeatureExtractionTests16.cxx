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
  REGISTER_TEST(otbFourierMellinDescriptorsNew);
  REGISTER_TEST(otbFourierMellinDescriptors);
  REGISTER_TEST(otbFourierMellinDescriptorsScaleInvariant);
  REGISTER_TEST(otbFourierMellinDescriptorsRotationInvariant);
  REGISTER_TEST(otbLocalHistogramImageFunctionNew);
  REGISTER_TEST(otbLocalHistogramImageFunctionTest);
  REGISTER_TEST(otbImageFunctionAdaptorNew);
  REGISTER_TEST(otbImageFunctionAdaptor);
  REGISTER_TEST(otbMetaImageFunctionNew);
  REGISTER_TEST(otbMetaImageFunction);
  REGISTER_TEST(otbHaralickTexturesImageFunctionNew);
  REGISTER_TEST(otbHaralickTexturesImageFunction);
  REGISTER_TEST(otbHistogramOfOrientedGradientCovariantImageFunctionNew);
  REGISTER_TEST(otbHistogramOfOrientedGradientCovariantImageFunction);
  REGISTER_TEST(otbRadiometricMomentsImageFilterNew);
  REGISTER_TEST(otbRadiometricMomentsImageFilter);
}
