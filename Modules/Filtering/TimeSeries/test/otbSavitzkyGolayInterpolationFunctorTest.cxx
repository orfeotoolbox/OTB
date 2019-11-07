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


#include "otbSavitzkyGolayInterpolationFunctor.h"

int otbSavitzkyGolayInterpolationFunctorTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  typedef float        PixelType;
  typedef unsigned int DoYType;
  const unsigned int   nbDates = 100;
  typedef itk::FixedArray<PixelType, nbDates> SeriesType;
  typedef itk::FixedArray<DoYType, nbDates>   DatesType;
  const unsigned int Radius = 2;


  // Create a time series
  SeriesType inSeries;
  SeriesType weightSeries;
  DatesType  doySeries;

  for (unsigned int i = 0; i < nbDates; ++i)
  {
    inSeries[i]     = 10 * std::cos(i / 10.0);
    doySeries[i]    = i;
    weightSeries[i] = 1;
  }

  // Set artifacts
  inSeries[nbDates / 4] = 0.0;
  inSeries[nbDates / 2] = 0.0;

  weightSeries[nbDates / 4] = 100000.0;
  weightSeries[nbDates / 2] = 100000.0;

  typedef otb::Functor::SavitzkyGolayInterpolationFunctor<Radius, SeriesType, DatesType, SeriesType> FunctorType;

  FunctorType f;
  f.SetWeights(weightSeries);
  f.SetDates(doySeries);

  SeriesType outSeries = f(inSeries);

  double interpolError = 0.0;

  for (unsigned int i = 0; i < nbDates; ++i)
  {

    interpolError += std::fabs(outSeries[i] - inSeries[i]);
  }

  interpolError /= nbDates;

  if (interpolError > 0.15)
  {
    std::cout << "Interpolation error = " << interpolError << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
