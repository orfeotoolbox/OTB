/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbMuellerToPolarisationDegreeAndPowerImageFilter);
  REGISTER_TEST(otbVectorMultiChannelsPolarimetricSynthesisFilter);
  REGISTER_TEST(otbReciprocalHAlphaImageFilter);
  REGISTER_TEST(otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew);
  REGISTER_TEST(otbReciprocalCovarianceToReciprocalCoherencyImageFilter);
  REGISTER_TEST(otbReciprocalHAlphaImageFilterNew);
  REGISTER_TEST(otbSinclairToCoherencyMatrixFunctor);
  REGISTER_TEST(otbPolarimetricSynthesisFunctor);
  REGISTER_TEST(otbMultiChannelsPolarimetricSynthesisFilter);
  REGISTER_TEST(otbSinclairImageFilterNew);
  REGISTER_TEST(otbSinclairImageFilter);
  REGISTER_TEST(otbReciprocalCovarianceToReciprocalCoherencyImageFilterNew);
  REGISTER_TEST(otbSinclairToCircularCovarianceMatrixFunctor);
  REGISTER_TEST(otbSinclairReciprocalImageFilterNew);
  REGISTER_TEST(otbSinclairReciprocalImageFilter);
  REGISTER_TEST(otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter);
  REGISTER_TEST(otbReciprocalCoherencyToReciprocalMuellerImageFilter);
  REGISTER_TEST(otbSinclairToReciprocalCovarianceMatrixFunctor);
  REGISTER_TEST(otbReciprocalCovarianceToCoherencyDegreeImageFilterNew);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceImageFilter);
  REGISTER_TEST(otbReciprocalCoherencyToReciprocalMuellerImageFilterNew);
  REGISTER_TEST(otbSinclairToMuellerMatrixFunctor);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceFunctor);
  REGISTER_TEST(otbPolarimetricDataNew);
  REGISTER_TEST(otbPolarimetricDataTest);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceImageFilterNew);
  REGISTER_TEST(otbSinclairToReciprocalCoherencyMatrixFunctor);
  REGISTER_TEST(otbSinclairToReciprocalCircularCovarianceMatrixFunctor);
  REGISTER_TEST(otbMultiChannelsPolarimetricSynthesisFilterNew);
  REGISTER_TEST(otbSinclairToCovarianceMatrixFunctor);
  REGISTER_TEST(otbReciprocalCovarianceToCoherencyDegreeImageFilter);
  REGISTER_TEST(otbMuellerToPolarisationDegreeAndPowerImageFilterNew);
  REGISTER_TEST(otbReciprocalBarnesDecompImageFilterNew);
  REGISTER_TEST(otbReciprocalBarnesDecompImageFilter);
  REGISTER_TEST(otbReciprocalHuynenDecompImageFilterNew);
  REGISTER_TEST(otbReciprocalHuynenDecompImageFilter);
  REGISTER_TEST(otbReciprocalPauliDecompImageFilterNew);
  REGISTER_TEST(otbReciprocalPauliDecompImageFilter);
}
