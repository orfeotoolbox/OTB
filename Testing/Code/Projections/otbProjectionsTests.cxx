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

// this file defines the otbProjectionsTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbProjectionBaseNew);
REGISTER_TEST(otbMapProjectionsNew);
REGISTER_TEST(otbSensorModelsNew);
REGISTER_TEST(otbCreateProjectionWithOSSIM);
REGISTER_TEST(otbCreateProjectionWithOTB);
REGISTER_TEST(otbCreateInverseForwardSensorModel);
REGISTER_TEST(otbRegionProjection);
REGISTER_TEST(otbRegionProjectionResampler);
REGISTER_TEST(otbOrthoRectificationFilter);
REGISTER_TEST(otbSensorImageToCarto);
REGISTER_TEST(otbSensorImageDEMToCarto);
}
