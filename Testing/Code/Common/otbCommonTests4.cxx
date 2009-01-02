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
REGISTER_TEST(otbHistogramStatisticsFunction);
REGISTER_TEST(otbListNew);
REGISTER_TEST(otbList);
REGISTER_TEST(otbObjectListNew);
REGISTER_TEST(otbObjectList);
REGISTER_TEST(otbObjectList2);
REGISTER_TEST(otbImageListNew);
REGISTER_TEST(otbImageList);
REGISTER_TEST(otbImageListSourceNew);
REGISTER_TEST(otbImageToImageListFilterNew);
REGISTER_TEST(otbImageListToImageFilterNew);
REGISTER_TEST(otbObjectListToObjectListFilterNew);
}
