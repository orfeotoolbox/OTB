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

// this file defines the otbSpatialReasoningTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(otbRCC8GraphSourceNew);
REGISTER_TEST(otbRCC8GraphFileWriterNew);
REGISTER_TEST(otbRCC8GraphFileWriter);
REGISTER_TEST(otbRCC8GraphFileReaderNew);
REGISTER_TEST(otbRCC8GraphFileReader);
REGISTER_TEST(otbRCC8GraphIOEndToEnd);
REGISTER_TEST(otbImageListToRCC8GraphFilterNew);
REGISTER_TEST(otbImageMultiSegmentationToRCC8GraphFilterNew);
REGISTER_TEST(otbImageMultiSegmentationToRCC8GraphFilter);
REGISTER_TEST(otbPolygonListToRCC8GraphFilterNew);
}
