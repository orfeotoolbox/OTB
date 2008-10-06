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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(ExtractRoadByStepsExampleTest);
REGISTER_TEST(ExtractRoadExampleTest);
REGISTER_TEST(SeamCarvingExampleTest);
REGISTER_TEST(SeamCarvingOtherExampleTest);
//REGISTER_TEST(ImageToSIFTKeyPointSetFilterTest);
REGISTER_TEST(ScaleInvariantFeatureImageFilterTest);
}

#undef main
#define main ExtractRoadByStepsExampleTest
#include "ExtractRoadByStepsExample.cxx"

#undef main
#define main ExtractRoadExampleTest
#include "ExtractRoadExample.cxx"

#undef main
#define main SeamCarvingExampleTest
#include "SeamCarvingExample.cxx"

#undef main
#define main SeamCarvingOtherExampleTest
#include "SeamCarvingOtherExample.cxx"

//#undef main
//#define main ImageToSIFTKeyPointSetFilterTest
//#include "ImageToSIFTKeyPointSetFilter.cxx"

#undef main
#define main ScaleInvariantFeatureImageFilterTest
#include "ScaleInvariantFeatureImageFilter.cxx"
