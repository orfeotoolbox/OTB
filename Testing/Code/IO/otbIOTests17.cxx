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

//Use only if OTB_USE_LIBLAS is on
void RegisterTests()
{
REGISTER_TEST(otbPointSetFileReaderNew);
REGISTER_TEST(otbPointSetFileReader);
REGISTER_TEST(otbPointSetFileReader2);
REGISTER_TEST(otbSpotImageMetadataInterfaceNew);
REGISTER_TEST(otbIkonosImageMetadataInterfaceNew);
REGISTER_TEST(otbQuickBirdImageMetadataInterfaceNew);
REGISTER_TEST(otbTerraSarImageMetadataInterfaceNew);
REGISTER_TEST(otbImageMetadataInterfaceTest);
REGISTER_TEST(otbImageMetadataInterfaceTest2);
}
