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
  REGISTER_TEST(otbDEMCaracteristicsExtractorNew);
  REGISTER_TEST(otbDEMCaracteristicsExtractor);
  REGISTER_TEST(otbAtmosphericCorrectionParametersNew);
  REGISTER_TEST(otbImageMetadataCorrectionParametersNew);
  REGISTER_TEST(otbFilterFunctionValuesTest);
  REGISTER_TEST(otbFilterFunctionValuesSpectralResponseTest);
  REGISTER_TEST(otbFilterFunctionValuesDigitalGlobeTest);
  REGISTER_TEST(otbSIXSTraitsTest);
  REGISTER_TEST(otbSIXSTraitsComputeAtmosphericParametersTest);
  REGISTER_TEST(otbAtmosphericRadiativeTermsNew);
  REGISTER_TEST(otbAtmosphericRadiativeTermsSingleChannelNew);
  REGISTER_TEST(otbAtmosphericRadiativeTermsTest);
  REGISTER_TEST(otbReflectanceToSurfaceReflectanceImageFilterNew);
  REGISTER_TEST(otbReflectanceToSurfaceReflectanceImageFilterTest);
  REGISTER_TEST(otbReflectanceToSurfaceReflectanceImageFilterTest2);
  REGISTER_TEST(otbSurfaceAdjacencyEffectCorrectionSchemeFilterNew);
  REGISTER_TEST(otbSurfaceAdjacencyEffectCorrectionSchemeFilter);
  REGISTER_TEST(otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter);
}
