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


#include "otbRandomPointSetSource.h"
#include "itkPointSet.h"

int otbRandomPointSetSourceTest(int, char* [])
{
  typedef double PixelType;
  typedef itk::PointSet<PixelType, 2> PointSetType;
  typedef otb::RandomPointSetSource<PointSetType> PointSetSourceType;
  typedef PointSetType::PointsContainer           PointsContainerType;

  PointSetSourceType::Pointer pointSet = PointSetSourceType::New();
  pointSet->SetNumberOfPoints(10);
  PointSetType::PointType minPoint, maxPoint;
  minPoint[0] = 10;
  minPoint[1] = 20;
  pointSet->SetMinPoint(minPoint);
  maxPoint[0] = 100;
  maxPoint[1] = 50;
  pointSet->SetMaxPoint(maxPoint);

  // To get deterministic results
  pointSet->SetSeed(100);

  pointSet->Update();

  // Get the the point container
  PointSetSourceType::PointsContainerPointer points = pointSet->GetOutput()->GetPoints();

  PointsContainerType::ConstIterator it = points->Begin();
  while (it != points->End())
  {
    PointSetType::PointType p = it.Value();
    std::cout.width(5);
    std::cout << p[0] << ", ";
    std::cout.width(5);
    std::cout << p[1] << std::endl;
    ++it;
  }

  // All objects should be automatically destroyed at this point

  return EXIT_SUCCESS;
}
