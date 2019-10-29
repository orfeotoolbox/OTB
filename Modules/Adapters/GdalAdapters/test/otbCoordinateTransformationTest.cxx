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

#include "otbCoordinateTransformation.h"

#include <iostream>
#include <stdexcept>

using namespace otb;

int otbCoordinateTransformationTest(int, char* [])
{
  bool success = true;

  SpatialReference inSR     = SpatialReference::FromDescription("EPSG:4326");
  SpatialReference outSR    = SpatialReference::FromDescription("EPSG:32631");
  SpatialReference outSRBis = SpatialReference::FromDescription("EPSG:2154");

  try
  {
    CoordinateTransformation transformation(inSR, outSR);

    CoordinateTransformation transformation2(transformation);
    CoordinateTransformation transformation3 = transformation;
    CoordinateTransformation transformation4(inSR, outSRBis);

    if (!(transformation == transformation2))
    {
      std::cerr << "Fail: transformation and transformation2 should be equal" << std::endl;
      success = false;
    }

    if (!(transformation == transformation3))
    {
      std::cerr << "Fail: transformation and transformation3 should be equal" << std::endl;
      success = false;
    }

    if (transformation == transformation4)
    {
      std::cerr << "Fail: transformation and transformation4 should be different" << std::endl;
      success = false;
    }

    if (transformation.GetSourceSpatialReference() != inSR)
    {
      std::cerr << "Fail: SourceSpatialReference from transform different from input spatial reference" << std::endl;
      success = false;
    }

    if (transformation.GetTargetSpatialReference() != outSR)
    {
      std::cerr << "Fail: TargetSpatialReference from transform different from input spatial reference" << std::endl;
      success = false;
    }

    std::cout << transformation << std::endl;

    double x = 43.60426;
    double y = 1.44367;
    double z = 300;
    double outX, outY, outZ;

    try
    {
      std::tie(outX, outY, outZ) = transformation.Transform(std::make_tuple(x, y, z));
    }
    catch (std::runtime_error& err)
    {
      std::cerr << "Fail: transformation call to 3D version of Transform() throws " << err.what() << std::endl;
      success = false;
    }

    try
    {
      std::tie(outX, outY) = transformation.Transform(std::make_tuple(x, y));
    }
    catch (std::runtime_error& err)
    {
      std::cerr << "Fail: transformation call to 2D version of Transform() throws " << err.what() << std::endl;
      success = false;
    }
  }
  catch (std::runtime_error& err)
  {
    std::cerr << "Exception raised during test: " << err.what() << std::endl;
    success = false;
  }

  if (success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
