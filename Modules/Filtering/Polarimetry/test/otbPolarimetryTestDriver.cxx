/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
  REGISTER_TEST(otbReciprocalCovarianceToReciprocalCoherencyImageFilter);
  REGISTER_TEST(otbSinclairToCoherencyMatrixFunctor);
  REGISTER_TEST(otbPolarimetricSynthesisFunctor);
  REGISTER_TEST(otbMultiChannelsPolarimetricSynthesisFilter);
  REGISTER_TEST(otbSinclairImageFilter);
  REGISTER_TEST(otbSinclairToCircularCovarianceMatrixFunctor);
  REGISTER_TEST(otbSinclairReciprocalImageFilter);
  REGISTER_TEST(otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter);
  REGISTER_TEST(otbReciprocalCoherencyToReciprocalMuellerImageFilter);
  REGISTER_TEST(otbSinclairToReciprocalCovarianceMatrixFunctor);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceImageFilter);
  REGISTER_TEST(otbSinclairToMuellerMatrixFunctor);
  REGISTER_TEST(otbMuellerToReciprocalCovarianceFunctor);
  REGISTER_TEST(otbPolarimetricDataTest);
  REGISTER_TEST(otbSinclairToReciprocalCoherencyMatrixFunctor);
  REGISTER_TEST(otbSinclairToReciprocalCircularCovarianceMatrixFunctor);
  REGISTER_TEST(otbSinclairToCovarianceMatrixFunctor);
  REGISTER_TEST(otbReciprocalCovarianceToCoherencyDegreeImageFilter);
  REGISTER_TEST(otbReciprocalBarnesDecompImageFilter);
  REGISTER_TEST(otbReciprocalHuynenDecompImageFilter);
  REGISTER_TEST(otbReciprocalPauliDecompImageFilter);
}
