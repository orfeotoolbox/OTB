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
  REGISTER_TEST(otbImageToSURFKeyPointSetFilterNew);
  REGISTER_TEST(otbImageToSURFKeyPointSetFilterOutputInterestPointAscii);
  REGISTER_TEST(otbImageToSURFKeyPointSetFilterOutputDescriptorAscii);
#ifdef OTB_USE_SIFTFAST
  REGISTER_TEST(otbImageToFastSIFTKeyPointSetFilterNew);
  REGISTER_TEST(otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii);
  REGISTER_TEST(otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii);
#endif
  REGISTER_TEST(otbKeyPointSetsMatchingFilterNew);
  REGISTER_TEST(otbKeyPointSetsMatchingFilter);
  REGISTER_TEST(otbLandmarkNew);
}
