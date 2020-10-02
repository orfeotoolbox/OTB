/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "itkVariableLengthVector.h"
#include "otbFlexibleDistanceWithMissingValue.h"

int otbFlexibleDistanceWithMissingValue(int itkNotUsed(argc), char* argv[])
{

  typedef itk::VariableLengthVector<double>                             VectorType;
  typedef otb::Statistics::FlexibleDistanceWithMissingValue<VectorType> DistanceType;

  const double epsilon(atof(argv[1]));

  float        a   = 2.0;
  float        b   = 0.5;
  unsigned int dim = 3;
  DistanceType::SetAlphaBeta(a, b);

  VectorType x(dim);
  x.Fill(1.);

  VectorType y(dim);
  y.Fill(2.);

  DistanceType::Pointer dist          = DistanceType::New();
  double                distanceValue = dist->Evaluate(x, y);
  std::cout << std::setprecision(20) << std::endl;
  std::cout << "dim, a, b          : " << dim << "," << a << "," << b << std::endl;
  std::cout << "dim*std::pow(3, b) : " << dim * std::pow(3, b) << std::endl;
  std::cout << "Distance         : " << distanceValue << std::endl;
  std::cout << "Epsilon          : " << epsilon << std::endl;
  std::cout << "-> Tests diff    : " << std::abs(distanceValue - dim * std::pow(3, b)) << std::endl;

  if (std::abs(distanceValue - dim * std::pow(3, b)) < epsilon)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
