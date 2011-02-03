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
  REGISTER_TEST(otbKmzProductWriterNew);
  REGISTER_TEST(otbKmzProductWriter);
  REGISTER_TEST(otbKmzProductWriter);
  REGISTER_TEST(otbTileMapImageIOHelperNew);
  REGISTER_TEST(otbTileMapImageIOHelperTest);
  REGISTER_TEST(otbKmzProductWriterWithLogoAndLegend);
  // Complex tests
  REGISTER_TEST(otbMonobandScalarToImageComplexFloat);
  REGISTER_TEST(otbMonobandScalarToImageComplexDouble);
  REGISTER_TEST(otbMonobandComplexToImageScalarFloat);
  REGISTER_TEST(otbMonobandComplexToImageScalarDouble);
  REGISTER_TEST(otbMonobandComplexToImageComplexFloat);
  REGISTER_TEST(otbMonobandComplexToImageComplexDouble);
  REGISTER_TEST(otbMonobandComplexToVectorImageScalarFloat);
  REGISTER_TEST(otbMonobandComplexToVectorImageScalarDouble);
  REGISTER_TEST(otbMonobandComplexToVectorImageComplexFloat);
  REGISTER_TEST(otbMonobandComplexToVectorImageComplexDouble);
  REGISTER_TEST(otbMultibandScalarToImageComplexFloat);
  REGISTER_TEST(otbMultibandScalarToImageComplexDouble);
  REGISTER_TEST(otbMultibandScalarToVectorImageComplexFloat);
  REGISTER_TEST(otbMultibandScalarToVectorImageComplexDouble);
  REGISTER_TEST(otbMultibandComplexToVectorImageScalarFloat);
  REGISTER_TEST(otbMultibandComplexToVectorImageScalarDouble);
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexFloat);
  REGISTER_TEST(otbMultibandComplexToVectorImageComplexDouble);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexFloat);
  REGISTER_TEST(otbMonobandScalarToVectorImageComplexDouble);
  REGISTER_TEST(otbMultibandComplexToImageScalarFloat);
  REGISTER_TEST(otbMultibandComplexToImageScalarDouble);

  // not deal in this commit
  //REGISTER_TEST(otbGDALReadPxlComplexDb);
  //REGISTER_TEST(otbGDALReadPxlComplexFl);
}
