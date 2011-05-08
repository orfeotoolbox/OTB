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
  REGISTER_TEST(otbLandsatTMIndexBright);
  REGISTER_TEST(otbLandsatTMIndexVis);
  REGISTER_TEST(otbLandsatTMIndexNIR);
  REGISTER_TEST(otbLandsatTMIndexMIR1);
  REGISTER_TEST(otbLandsatTMIndexMIR2);
  REGISTER_TEST(otbLandsatTMIndexTIR);
  REGISTER_TEST(otbLandsatTMIndexMIRTIR);
  REGISTER_TEST(otbLandsatTMIndexNDVI);
  REGISTER_TEST(otbLandsatTMIndexNDBSI);
  REGISTER_TEST(otbLandsatTMIndexBIO);
  REGISTER_TEST(otbLandsatTMIndexNDSI);
  REGISTER_TEST(otbLandsatTMIndexNDSIVis);
  REGISTER_TEST(otbLandsatTMIndexNDBBBI);
  REGISTER_TEST(otbLandsatTMLinguisticLabels);
  REGISTER_TEST(otbLandsatTMKernelSpectralRules);
  REGISTER_TEST(otbLandsatTMKernelSpectralRulesWithImage);
  REGISTER_TEST(otbLandsatTMBrightTest);
  REGISTER_TEST(otbLandsatTMThickCloudTest);
  REGISTER_TEST(otbLandsatTMThinCloudTest);
  REGISTER_TEST(otbLandsatTMVegetationTest);
  REGISTER_TEST(otbLandsatTMSpectralRuleBasedClassifierTest);
}
