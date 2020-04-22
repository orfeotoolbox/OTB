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


#include "itkPointSet.h"
#include "otbPointSetSource.h"
#include "itkMacro.h"

int otbPointSetSourceTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  // Declare the PointSet pixel type.
  // Those are the values associated
  // with each PointSet point. (not used on this filter test)
  typedef int PixelType;

  // Declare the types of the PointSet
  typedef itk::PointSet<PixelType, 2> PointSetType;

  // Declare the type for PointsContainer
  typedef PointSetType::PointsContainer PointsContainerType;

  // Declare the type for PointsContainerPointer
  typedef PointSetType::PointsContainerPointer PointsContainerPointer;
  // Declare the type for Points
  typedef PointSetType::PointType PointType;

  // Create an input Point Set
  PointSetType::Pointer inputPointSet = PointSetType::New();

  // Insert data on the PointSet
  PointsContainerPointer points = inputPointSet->GetPoints();

  // Fill a square with points , just to get some data
  int                                    n     = 1; // let's start with a few of them
  PointsContainerType::ElementIdentifier count = 0; // count them

  for (int x = -n; x <= n; x++)
  {
    for (int y = -n; y <= n; y++)
    {
      PointType p;
      p[0] = x;
      p[1] = y;
      std::cout << "Inserting point # ";
      std::cout.width(3);
      std::cout << count << "  = ";
      std::cout.width(4);
      std::cout << p[0] << ", ";
      std::cout.width(4);
      std::cout << p[1] << std::endl;
      points->InsertElement(count, p);
      count++;
    }
  }

  std::cout << "Input PointSet has " << inputPointSet->GetNumberOfPoints();
  std::cout << "   points " << std::endl;

  // All objects should be automatically destroyed at this point

  return EXIT_SUCCESS;
}
