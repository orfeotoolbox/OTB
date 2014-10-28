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
  REGISTER_TEST(otbAlignImageToPath);
  REGISTER_TEST(otbDrawPathDessinCarre);
  REGISTER_TEST(otbDrawPathAlign);
  REGISTER_TEST(otbComplexMomentsImageFunctionNew);
  REGISTER_TEST(otbComplexMomentsImageFunction);
  REGISTER_TEST(otbComplexMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbRealMomentsImageFunctionNew);
  REGISTER_TEST(otbRealMomentsImageFunction);
  REGISTER_TEST(otbRadiometricMomentsImageFunctionNew);
  REGISTER_TEST(otbRadiometricMomentsImageFunction);
  REGISTER_TEST(otbHuMomentsImageFunctionNew);
  REGISTER_TEST(otbHuMomentsImageFunction);
  REGISTER_TEST(otbHuMomentsImageFunctionRotationInvariant);
  REGISTER_TEST(otbHuMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbFlusserMomentsImageFunctionNew);
  REGISTER_TEST(otbFlusserMomentsImageFunction);
  REGISTER_TEST(otbFlusserMomentsImageFunctionRotationInvariant);
  REGISTER_TEST(otbFlusserMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbComplexMomentPathNew);
  REGISTER_TEST(otbComplexMomentPath);
  REGISTER_TEST(otbComplexMomentPathFloat);
  REGISTER_TEST(otbHuPathNew);
  REGISTER_TEST(otbHuPath);
}
