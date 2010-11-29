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
  REGISTER_TEST(otbMapFileProductWriterNew);
  REGISTER_TEST(otbMapFileProductWriter);
  REGISTER_TEST(otbTileMapImageIOHelperNew);
  REGISTER_TEST(otbTileMapImageIOHelperTest);
  // Complex tests
  REGISTER_TEST(otbImageDoubleToImageComplex);
  REGISTER_TEST(otbImageComplexToImageDouble);
  REGISTER_TEST(otbImageComplexToImageComplex);
  REGISTER_TEST(otbImageComplexToVectorImageDouble);
  REGISTER_TEST(otbImageComplexToVectorImageComplex);
}
