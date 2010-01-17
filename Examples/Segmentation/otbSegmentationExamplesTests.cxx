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
  REGISTER_TEST(IsolatedConnectedImageFilterTest);
  REGISTER_TEST(FastMarchingImageFilterTest);
  REGISTER_TEST(OtsuThresholdImageFilterTest);
  REGISTER_TEST(ConfidenceConnectedTest);
  REGISTER_TEST(NeighborhoodConnectedImageFilterTest);
  REGISTER_TEST(WatershedSegmentationTest);
  REGISTER_TEST(ConnectedThresholdImageFilterTest);
  REGISTER_TEST(OtsuMultipleThresholdImageFilterTest);
  REGISTER_TEST(LabelizeNeighborhoodConnectedImageFilterTest);
}

#undef main
#define main IsolatedConnectedImageFilterTest
#include "IsolatedConnectedImageFilter.cxx"

#undef main
#define main FastMarchingImageFilterTest
#include "FastMarchingImageFilter.cxx"

#undef main
#define main OtsuThresholdImageFilterTest
#include "OtsuThresholdImageFilter.cxx"

#undef main
#define main ConfidenceConnectedTest
#include "ConfidenceConnected.cxx"

#undef main
#define main NeighborhoodConnectedImageFilterTest
#include "NeighborhoodConnectedImageFilter.cxx"

#undef main
#define main WatershedSegmentationTest
#include "WatershedSegmentation.cxx"

#undef main
#define main ConnectedThresholdImageFilterTest
#include "ConnectedThresholdImageFilter.cxx"

#undef main
#define main OtsuMultipleThresholdImageFilterTest
#include "OtsuMultipleThresholdImageFilter.cxx"

#undef main
#define main LabelizeNeighborhoodConnectedImageFilterTest
#include "LabelizeNeighborhoodConnectedImageFilter.cxx"
