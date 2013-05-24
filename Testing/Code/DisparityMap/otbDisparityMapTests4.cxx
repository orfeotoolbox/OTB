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

// this file defines the otbDisparityMapTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbAdhesionCorrectionFilterNew);
  REGISTER_TEST(otbAdhesionCorrectionFilter);
  REGISTER_TEST(otbDisparityMapMedianFilterNew);
  REGISTER_TEST(otbDisparityMapMedianFilter);
  REGISTER_TEST(otbDisparityMapToDEMFilterNew);
  REGISTER_TEST(otbDisparityMapToDEMFilter);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilterNew);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilter);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilterNCC);
  REGISTER_TEST(otbSubPixelDisparityImageFilter);
  REGISTER_TEST(otbSubPixelDisparityImageFilterNew);
  REGISTER_TEST(otbDisparityMapTo3DFilter);
  REGISTER_TEST(otbDisparityMapTo3DFilterNew);
  REGISTER_TEST(otbMultiDisparityMapTo3DFilter);
  REGISTER_TEST(otbMultiDisparityMapTo3DFilterNew);
  REGISTER_TEST(otbMulti3DMapToDEMFilter);
  REGISTER_TEST(otbMulti3DMapToDEMFilterUTM);
  REGISTER_TEST(otbMulti3DMapToDEMFilterManual);
  REGISTER_TEST(otbMulti3DMapToDEMFilterNew);
  REGISTER_TEST(otbBijectionCoherencyFilter);
  REGISTER_TEST(otbBijectionCoherencyFilterNew);
  REGISTER_TEST(otbDisparityTranslateFilter);
  REGISTER_TEST(otbDisparityTranslateFilterNew);
}
