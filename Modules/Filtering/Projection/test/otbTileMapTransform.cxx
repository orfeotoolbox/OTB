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


#include <fstream>
#include <iomanip>
#include "otbTileMapTransform.h"

int otbTileMapTransform(int itkNotUsed(argc), char* argv[])
{
  const char*   outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  typedef otb::TileMapTransform<otb::TransformDirection::FORWARD> ProjectionType;
  ProjectionType::Pointer                                         projection = ProjectionType::New();
  projection->SetLevel(10);

  file << std::setprecision(15);
  file << "Forward projection: level " << projection->GetLevel() << std::endl;
  itk::Point<double, 2> point;
  point[0] = 0; // lon
  point[1] = 0; // lat

  file << point << " -> ";
  file << projection->TransformPoint(point);
  file << std::endl << std::endl;

  point[0] = 1.48;  // lon
  point[1] = 43.55; // lat

  file << point << " -> ";
  file << projection->TransformPoint(point);
  file << std::endl << std::endl;

  typedef otb::TileMapTransform<otb::TransformDirection::INVERSE> InverseProjectionType;
  InverseProjectionType::Pointer                                  inverseProjection = InverseProjectionType::New();
  inverseProjection->SetLevel(10);

  file << "Inverse projection: level " << inverseProjection->GetLevel() << std::endl;

  point[0] = 132149; // X
  point[1] = 95774;  // Y

  file << point << " -> ";
  file << inverseProjection->TransformPoint(point);
  file << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
