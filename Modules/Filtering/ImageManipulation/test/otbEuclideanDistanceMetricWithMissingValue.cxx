/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "itkVariableLengthVector.h"
#include "otbEuclideanDistanceMetricWithMissingValue.h"

int otbEuclideanDistanceMetricWithMissingValue(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef itk::VariableLengthVector<double>                              VectorType;
  typedef otb::Statistics::EuclideanDistanceMetricWithMissingValue<VectorType> DistanceType;

  DistanceType::Pointer dist = DistanceType::New();

  if (dist->IsMissingValue(0.0)) return EXIT_FAILURE;

  if (dist->IsMissingValue(1.0)) return EXIT_FAILURE;

  if (dist->IsMissingValue(-1.0)) return EXIT_FAILURE;

  /*
    std::cerr << "Is O Nan ?? -> " << dist->IsMissingValue( 0.0 ) << "\n";
    std::cerr << "Is 1 Nan ?? -> " << dist->IsMissingValue( 1.0 ) << "\n";
    std::cerr << "Is -1 Nan ?? -> " << dist->IsMissingValue( -1.0 ) << "\n";
  */

  double x = 0.0;

  dist->SetToMissingValue(x);
  if (!dist->IsMissingValue(x)) return EXIT_FAILURE;
  // std::cerr << "Is x (=" << x << ") Nan ?? -> " << dist->IsMissingValue( x ) << "\n";

  return EXIT_SUCCESS;
}
