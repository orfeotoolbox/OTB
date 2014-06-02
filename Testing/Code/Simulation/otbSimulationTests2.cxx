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
  REGISTER_TEST(otbSpectralResponseNew);
  REGISTER_TEST(otbSpectralResponse);
  REGISTER_TEST(otbSatelliteRSRNew);
  REGISTER_TEST(otbSatelliteRSR);
  REGISTER_TEST(otbSatelliteRSRCheckValue);
  REGISTER_TEST(otbReduceSpectralResponseNew);
  REGISTER_TEST(otbReduceSpectralResponse);
  REGISTER_TEST(otbReduceSpectralResponseClassifierRAndNIR);
  REGISTER_TEST(otbAtmosphericEffects);
  REGISTER_TEST(otbSpatialisationTestNew);
  REGISTER_TEST(otbSpatialisationTest);
  REGISTER_TEST(otbLabelMapToSimulatedImageFilterTest);
  REGISTER_TEST(otbImageSimulationMethodWithVectorDataTest);
  REGISTER_TEST(otbImageSimulationMethodWithSpatialisationTest);
}
