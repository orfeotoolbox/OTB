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

#include "itkMacro.h"
#include "otbSarBrightnessFunctor.h"
#include "otbMath.h"

int otbSarBrightnessFunctorWithoutNoise(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef double ScalarType;

  typedef otb::Functor::SarBrightnessFunctor<ScalarType, ScalarType> FunctorType;

  FunctorType functWithNoise;
  FunctorType functWithoutNoise;

  // With Noise
  functWithNoise.SetNoise(0.0);
  if (std::abs(functWithNoise.GetNoise() - 0.0) > 0.0)
  {
    return false;
  }
  functWithNoise.SetScale(10.0);
  if (std::abs(functWithNoise.GetScale() - 10.0) > 0.0)
  {
    return false;
  }
  functWithNoise.SetAntennaPatternNewGain(10.0);
  if (std::abs(functWithNoise.GetAntennaPatternNewGain() - 10.0) > 0.0)
  {
    return false;
  }
  functWithNoise.SetAntennaPatternOldGain(10.0);
  if (std::abs(functWithNoise.GetAntennaPatternOldGain() - 10.0) > 0.0)
  {
    return false;
  }

  functWithNoise.SetRangeSpreadLoss(10.0);
  if (std::abs(functWithNoise.GetRangeSpreadLoss() - 10.0) > 0.0)
  {
    return false;
  }

  // Without Noise
  functWithoutNoise.SetScale(10.0);
  if (std::abs(functWithoutNoise.GetScale() - 10.0) > 0.0)
  {
    return false;
  }
  functWithoutNoise.SetAntennaPatternNewGain(10.0);
  if (std::abs(functWithoutNoise.GetAntennaPatternNewGain() - 10.0) > 0.0)
  {
    return false;
  }
  functWithoutNoise.SetAntennaPatternOldGain(10.0);
  if (std::abs(functWithoutNoise.GetAntennaPatternOldGain() - 10.0) > 0.0)
  {
    return false;
  }

  functWithoutNoise.SetRangeSpreadLoss(10.0);
  if (std::abs(functWithoutNoise.GetRangeSpreadLoss() - 10.0) > 0.0)
  {
    return false;
  }

  std::cout << "First BetaNaught (With Noise = 0): " << functWithNoise.operator()(0.) << " *** "
            << "First BetaNaught (Without Noise): " << functWithoutNoise.operator()(0.) << std::endl;
  std::cout << "Second BetaNaught (With Noise = 0): " << functWithNoise.operator()(1.) << " *** "
            << "Second BetaNaught (Without Noise): " << functWithoutNoise.operator()(1.) << std::endl;

  return EXIT_SUCCESS;
}
