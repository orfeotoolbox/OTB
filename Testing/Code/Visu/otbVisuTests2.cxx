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
REGISTER_TEST(otbImageViewerWithMultiBandFilter);
REGISTER_TEST(otbDoubleImageViewerWithLink);
REGISTER_TEST(otbImageToGrayscaleAnaglyphImageFilterNew);
REGISTER_TEST(otbImageToGrayscaleAnaglyphImageFilter);
REGISTER_TEST(otbVectorImageToColorAnaglyphVectorImageFilterNew);
REGISTER_TEST(otbVectorImageToColorAnaglyphVectorImageFilter);
REGISTER_TEST(otbAlternateViewerTest);
REGISTER_TEST(otbImageWidgetPolygonFormNew);
REGISTER_TEST(otbImageWidgetPolygonForm);
REGISTER_TEST(otbHistogramAndTransferFunctionWidgetNew);
REGISTER_TEST(otbHistogramAndTransferFunctionWidget);
REGISTER_TEST(otbImageWidgetTransferFunctionsNew);
REGISTER_TEST(otbImageWidgetTransferFunctions);
REGISTER_TEST(otbImageViewerWithOverlay);
}
