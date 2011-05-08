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


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbLUMImageIOTestCanRead);
  REGISTER_TEST(otbLUMImageIOTestCanWrite);
  REGISTER_TEST(otbMWImageIOTestCanRead);
  REGISTER_TEST(otbMWImageIOTestCanWrite);
  REGISTER_TEST(otbImageFileReaderTest);
  REGISTER_TEST(otbImageFileReaderTestFloat);
  REGISTER_TEST(otbShortImageIOTest);
  REGISTER_TEST(otbImageIOFactoryNew);
  REGISTER_TEST(otbImageFileReaderWithComplexPixelTest);
  REGISTER_TEST(otbCompareWritingComplexImageTest);
  REGISTER_TEST(otbVectorImageFileReaderWithComplexPixelTest);
}
