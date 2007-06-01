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
REGISTER_TEST(CorrelChDetTest);
REGISTER_TEST(DiffChDetTest);
REGISTER_TEST(RatioChDetTest);
REGISTER_TEST(KullbackLeiblerDistanceChDetTest);
}

#undef main
#define main CorrelChDetTest
#include "CorrelChDet.cxx"

#undef main
#define main DiffChDetTest
#include "DiffChDet.cxx"

#undef main
#define main RatioChDetTest
#include "RatioChDet.cxx"

#undef main
#define main KullbackLeiblerDistanceChDetTest
#include "KullbackLeiblerDistanceChDet.cxx"
