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
  REGISTER_TEST(otbSpotImageMetadataInterfaceNew);
  REGISTER_TEST(otbPleiadesImageMetadataInterfaceNew);
  REGISTER_TEST(otbFormosatImageMetadataInterfaceNew);
  REGISTER_TEST(otbIkonosImageMetadataInterfaceNew);
  REGISTER_TEST(otbQuickBirdImageMetadataInterfaceNew);
  REGISTER_TEST(otbWorldView2ImageMetadataInterfaceNew);
  REGISTER_TEST(otbTerraSarImageMetadataInterfaceNew);
  REGISTER_TEST(otbImageMetadataInterfaceBaseTest);
  REGISTER_TEST(otbOpticalImageMetadataInterfaceTest);
  REGISTER_TEST(otbSarImageMetadataInterfaceTest);
  REGISTER_TEST(otbImageMetadataInterfaceTest2);
}
