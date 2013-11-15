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
  REGISTER_TEST(otbPolarimetricSynthesisFunctor);
  REGISTER_TEST(otbMultiChannelsPolarimetricSynthesisFilterNew);
  REGISTER_TEST(otbMultiChannelsPolarimetricSynthesisFilter);
  REGISTER_TEST(otbVectorMultiChannelsPolarimetricSynthesisFilter);
  REGISTER_TEST(otbSinclairToCircularCovarianceMatrixFunctor);
  REGISTER_TEST(otbSinclairToMuellerMatrixFunctor);
  REGISTER_TEST(otbSinclairToReciprocalCovarianceMatrixFunctor);
  REGISTER_TEST(otbSinclairToCoherencyMatrixFunctor);
  REGISTER_TEST(otbSinclairToReciprocalCircularCovarianceMatrixFunctor);
  REGISTER_TEST(otbSinclairToReciprocalCoherencyMatrixFunctor);
  REGISTER_TEST(otbSinclairToCovarianceMatrixFunctor);
  REGISTER_TEST(otbSinclairImageFilterNew);
  REGISTER_TEST(otbSinclairImageFilter);
  REGISTER_TEST(otbSinclairReciprocalImageFilterNew);
  REGISTER_TEST(otbSinclairReciprocalImageFilter);
}
