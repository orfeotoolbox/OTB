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

#include "itkPoint.h"
#include "otbGeographicalDistance.h"


int otbGeographicalDistance(int itkNotUsed(argc), char* argv[])
{
  typedef itk::Point<double, 2> PointType;
  typedef otb::GeographicalDistance<PointType> GeoDistanceType;

  GeoDistanceType::Pointer geoDistance = GeoDistanceType::New();

  PointType a, b;
  a[0] = atof(argv[1]);
  a[1] = atof(argv[2]);
  b[0] = atof(argv[3]);
  b[1] = atof(argv[4]);

  GeoDistanceType::OriginType o(2);
  o[0] = b[0];
  o[1] = b[1];

  double expectedDistance = atof(argv[5]);
  double tolerance        = atof(argv[6]);

  double distEvaluate2     = geoDistance->Evaluate(a, b);
  double distEvaluate2Null = geoDistance->Evaluate(a, a);

  geoDistance->SetOrigin(o);
  double distEvaluate1     = geoDistance->Evaluate(a);
  double distEvaluate1Null = geoDistance->Evaluate(b);

  // Check results
  bool failFlag = false;
  std::cerr.setf(std::ios_base::fixed);
  std::cerr.precision(5);
  if (std::abs(distEvaluate1 - expectedDistance) > tolerance)
  {
    std::cerr << "Evaluate(" << a << ") with origin " << o << " is inaccurate: expected " << expectedDistance << ", found " << distEvaluate1 << std::endl;
    failFlag = true;
  }

  if (std::abs(distEvaluate2 - expectedDistance) > tolerance)
  {
    std::cerr << "Evaluate(" << a << ", " << b << ") is inaccurate: expected " << expectedDistance << ", found " << distEvaluate2 << std::endl;
    failFlag = true;
  }

  if (distEvaluate2Null > tolerance)
  {
    std::cerr << "Evaluate(" << a << ", " << a << ") is inaccurate: expected 0., found " << distEvaluate2Null << std::endl;
    failFlag = true;
  }

  if (distEvaluate1Null > tolerance)
  {
    std::cerr << "Evaluate(" << b << ") with origin " << o << " is inaccurate: expected 0., found " << distEvaluate2Null << std::endl;
    failFlag = true;
  }

  if (failFlag)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
