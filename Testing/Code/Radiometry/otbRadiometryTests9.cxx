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
  REGISTER_TEST(otbTerraSarBrightnessFunctor);
  REGISTER_TEST(otbTerraSarCalibrationFunctor);
  REGISTER_TEST(otbTerraSarCalibrationImageFilterNew);
  REGISTER_TEST(otbTerraSarCalibrationImageFilterTest);
  REGISTER_TEST(otbTerraSarCalibrationImageComplexFilterTest);
  REGISTER_TEST(otbTerraSarBrightnessImageFilterNew);
  REGISTER_TEST(otbTerraSarBrightnessImageFilterTest);
  REGISTER_TEST(otbTerraSarBrightnessImageComplexFilterTest);
  REGISTER_TEST(otbSarParametricMapFunctionTest);
  REGISTER_TEST(otbSarParametricMapFunctionToImageFilter);
  REGISTER_TEST(otbSarRadiometricCalibrationFunctor);
  REGISTER_TEST(otbSarRadiometricCalibrationFunction);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithRealPixelTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest);
  REGISTER_TEST(otbSarBrightnessFunctor);
  REGISTER_TEST(otbSarBrightnessFunction);
  REGISTER_TEST(otbSarBrightnessToImageFilterTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterCompareTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithExtractROIBeforeTest);
}
