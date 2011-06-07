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
  REGISTER_TEST(otbReciprocalCovarianceToReciprocalCoherencyImageFilterNew);
  REGISTER_TEST(otbReciprocalCovarianceToReciprocalCoherencyImageFilter);
  REGISTER_TEST(otbReciprocalCovarianceToCoherencyDegreeImageFilterNew);
  REGISTER_TEST(otbReciprocalCovarianceToCoherencyDegreeImageFilter);
  REGISTER_TEST(otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew);
  REGISTER_TEST(otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter);
  REGISTER_TEST(otbReciprocalHAlphaImageFilterNew);
  REGISTER_TEST(otbReciprocalHAlphaImageFilter);
  REGISTER_TEST(otbReciprocalCoherencyToReciprocalMuellerImageFilterNew);
  REGISTER_TEST(otbReciprocalCoherencyToReciprocalMuellerImageFilter);
  REGISTER_TEST(otbMuellerToPolarisationDegreeAndPowerImageFilterNew);
  REGISTER_TEST(otbMuellerToPolarisationDegreeAndPowerImageFilter);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceFunctor);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceImageFilterNew);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceImageFilter);
  REGISTER_TEST(otbPolarimetricDataNew);
  REGISTER_TEST(otbPolarimetricDataTest);
}
