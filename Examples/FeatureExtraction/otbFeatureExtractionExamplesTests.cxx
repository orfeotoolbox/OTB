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
REGISTER_TEST(CorrelationLineDetectorExampleTest);
REGISTER_TEST(LocalHoughExampleTest);
REGISTER_TEST(HarrisExampleTest);
REGISTER_TEST(TouziEdgeDetectorExampleTest);
REGISTER_TEST(ExtractSegmentsByStepsExampleTest);
REGISTER_TEST(AssymmetricFusionOfLineDetectorExampleTest);
REGISTER_TEST(ExtractSegmentsExampleTest);
REGISTER_TEST(RatioLineDetectorExampleTest);
REGISTER_TEST(AlignmentsExampleTest);
REGISTER_TEST(ExtractRoadByStepsExampleTest);
}

#undef main
#define main CorrelationLineDetectorExampleTest
#include "CorrelationLineDetectorExample.cxx"

#undef main
#define main LocalHoughExampleTest
#include "LocalHoughExample.cxx"

#undef main
#define main HarrisExampleTest
#include "HarrisExample.cxx"

#undef main
#define main TouziEdgeDetectorExampleTest
#include "TouziEdgeDetectorExample.cxx"

#undef main
#define main ExtractSegmentsByStepsExampleTest
#include "ExtractSegmentsByStepsExample.cxx"

#undef main
#define main AssymmetricFusionOfLineDetectorExampleTest
#include "AssymmetricFusionOfLineDetectorExample.cxx"

#undef main
#define main ExtractSegmentsExampleTest
#include "ExtractSegmentsExample.cxx"

#undef main
#define main RatioLineDetectorExampleTest
#include "RatioLineDetectorExample.cxx"

#undef main
#define main AlignmentsExampleTest
#include "AlignmentsExample.cxx"

#undef main
#define main ExtractRoadsByStepsExampleTest
#include "ExtractRoadByStepsExample.cxx"

