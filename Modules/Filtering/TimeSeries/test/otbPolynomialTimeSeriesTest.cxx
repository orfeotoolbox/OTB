/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbTimeSeries.h"

int otbPolynomialTimeSeriesTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  typedef double     CoefficientPrecisionType;
  const unsigned int Degree = 2;
  typedef otb::PolynomialTimeSeries<Degree, CoefficientPrecisionType> FunctionType;
  typedef FunctionType::CoefficientsType CoefficientsType;

  CoefficientsType coefs;

  coefs[0] = 1;
  coefs[1] = 2;
  coefs[2] = 4;

  FunctionType f;
  f.SetCoefficients(coefs);

  for (unsigned int i = 0; i <= Degree; ++i)
    if (coefs[i] != f.GetCoefficient(i))
      return EXIT_FAILURE;

  int VALMAX = 10;
  for (int val = -VALMAX; val <= VALMAX; ++val)
    if (f.GetValue(val) != (coefs[0] + coefs[1] * val + coefs[2] * val * val))
      return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
