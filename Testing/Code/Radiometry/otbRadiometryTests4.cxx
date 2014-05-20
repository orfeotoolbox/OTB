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
  REGISTER_TEST(otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew);
  REGISTER_TEST(otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms);
  REGISTER_TEST(otbAtmosphericCorrectionParametersToAtmosphericRadiativeTerms);
  REGISTER_TEST(otbAtmosphericCorrectionSequencementTest);
  REGISTER_TEST(otbEVIRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbTSARVIRAndBAndNIRVegetationIndexImageFilter);
  REGISTER_TEST(otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter);
}
