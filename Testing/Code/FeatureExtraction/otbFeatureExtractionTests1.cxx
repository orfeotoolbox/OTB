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
  REGISTER_TEST(otbAlignImageToPath);
  REGISTER_TEST(otbDrawPathDessinCarre);
  REGISTER_TEST(otbDrawPathAlign);
  REGISTER_TEST(otbComplexMomentImageNew);
  REGISTER_TEST(otbComplexMomentImage);
  REGISTER_TEST(otbComplexMomentImageScaleInvariant);
  REGISTER_TEST(otbRealMomentImageNew);
  REGISTER_TEST(otbRealMomentImage);
  REGISTER_TEST(otbRadiometricMomentsImageNew);
  REGISTER_TEST(otbRadiometricMomentsImage);
  REGISTER_TEST(otbHuImage);
  REGISTER_TEST(otbHuImageRotationInvariant);
  REGISTER_TEST(otbHuImageScaleInvariant);
  REGISTER_TEST(otbFlusserImage);
  REGISTER_TEST(otbFlusserImageRotationInvariant);
  REGISTER_TEST(otbFlusserImageScaleInvariant);
  REGISTER_TEST(otbComplexMomentPathNew);
  REGISTER_TEST(otbComplexMomentPath);
  REGISTER_TEST(otbComplexMomentPathFloat);
  REGISTER_TEST(otbHuPathNew);
  REGISTER_TEST(otbHuPath);
}
