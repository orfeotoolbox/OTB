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

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests

#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(LeeImageFilterTest);
  REGISTER_TEST(FrostImageFilterTest);
  REGISTER_TEST(DEMToRainbowExampleTest);
  REGISTER_TEST(ScalingFilterExampleTest);
  REGISTER_TEST(PrintableImageFilterExample1Test);
  REGISTER_TEST(PrintableImageFilterExample2Test);
  REGISTER_TEST(IndexedToRGBExampleTest);
  REGISTER_TEST(MeanShiftSegmentationFilterExampleTest);
  REGISTER_TEST(HillShadingExampleTest);
}

#undef main
#define main LeeImageFilterTest
#include "LeeImageFilter.cxx"

#undef main
#define main FrostImageFilterTest
#include "FrostImageFilter.cxx"

#undef main
#define main DEMToRainbowExampleTest
#include "DEMToRainbowExample.cxx"

#undef main
#define main ScalingFilterExampleTest
#include "ScalingFilterExample.cxx"

#undef main
#define main PrintableImageFilterExample1Test
#include "PrintableImageFilterExample.cxx"

#undef main
#define main PrintableImageFilterExample2Test
#include "PrintableImageFilterExample.cxx"

#undef main
#define main IndexedToRGBExampleTest
#include "IndexedToRGBExample.cxx"

#undef main
#define main MeanShiftSegmentationFilterExampleTest
#include "MeanShiftSegmentationFilterExample.cxx"

#undef main
#define main HillShadingExampleTest
#include "HillShadingExample.cxx"
