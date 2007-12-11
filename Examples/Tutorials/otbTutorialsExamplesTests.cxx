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
  REGISTER_TEST(HelloWorldOTBTest);
  REGISTER_TEST(TutorialsPipelineTest);
  REGISTER_TEST(TutorialsFilteringPipelineTest);
  REGISTER_TEST(TutorialsScalingPipelineTest);
  REGISTER_TEST(TutorialsSmarterFilteringPipelineTest);
}

#undef main
#define main HelloWorldOTBTest
#include "HelloWorldOTBT.cxx"

#undef main
#define main TutorialsPipelineTest
#include "Pipeline.cxx"

#undef main
#define main TutorialsFilteringPipelineTest
#include "FilteringPipeline.cxx"

#undef main
#define main TutorialsScalingPipelineTest
#include "ScalingPipeline.cxx"

#undef main
#define main TutorialsSmarterFilteringPipelineTest
#include "SmarterFilteringPipeline.cxx"

