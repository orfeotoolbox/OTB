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

#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(otbHarrisToPointSet);
REGISTER_TEST(otbPixelSuppressionByDirectionNew);
REGISTER_TEST(otbPixelSuppressionByDirection);
REGISTER_TEST(otbFillGapsFilterNew);
REGISTER_TEST(otbFillGapsFilter);
REGISTER_TEST(otbLocalHoughNew);
REGISTER_TEST(otbLocalHough);
REGISTER_TEST(otbLocalHoughDraw);
REGISTER_TEST(otbModulusAndDirectionImageFiltersNew);
REGISTER_TEST(otbHoughTransform2DLinesImageTest);
}
