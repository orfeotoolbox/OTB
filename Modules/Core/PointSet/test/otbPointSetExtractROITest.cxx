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


#include "otbPointSetExtractROI.h"
#include "itkPointSet.h"
#include "otbRandomPointSetSource.h"

int otbPointSetExtractROITest(int, char* [])
{
  typedef float PixelType;
  typedef itk::PointSet<PixelType, 2> PointSetType;
  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointSetType::PointType       PointType;

  typedef otb::RandomPointSetSource<PointSetType> PointSetSource;

  PointSetSource::Pointer pointSetSource = PointSetSource::New();
  pointSetSource->SetNumberOfPoints(1000);
  PointSetType::PointType maxPoint;
  maxPoint[0] = 1000;
  maxPoint[1] = 1000;
  pointSetSource->SetMaxPoint(maxPoint);
  pointSetSource->SetSeed(152002);

  pointSetSource->Update();

  std::cout << "Input PointSet has " << pointSetSource->GetOutput()->GetNumberOfPoints();
  std::cout << " points " << std::endl;

  typedef otb::PointSetExtractROI<PointSetType, PointSetType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(pointSetSource->GetOutput());
  filter->SetStartX(450);
  filter->SetStartY(450);
  filter->SetSizeX(100);
  filter->SetSizeY(100);
  filter->Update();

  std::cout << "Keeping points from " << filter->GetStartX() << ", " << filter->GetStartY() << std::endl;
  std::cout << "with a region size of " << filter->GetSizeX() << ", " << filter->GetSizeY() << std::endl;

  // Output the result

  // Get the Smart Pointer to the Filter Output
  PointSetType::Pointer outputPointSet = filter->GetOutput();

  std::cout << "Output PointSet has " << outputPointSet->GetNumberOfPoints();
  std::cout << " points " << std::endl;

  // Get the the point container
  PointSetType::PointsContainerPointer extractedPoints = outputPointSet->GetPoints();

  PointsContainerType::ConstIterator it = extractedPoints->Begin();
  while (it != extractedPoints->End())
  {
    PointType p = it.Value();
    std::cout.width(5);
    std::cout << p[0] << ", ";
    std::cout.width(5);
    std::cout << p[1] << std::endl;
    ++it;
  }

  // All objects should be automatically destroyed at this point

  if (outputPointSet->GetNumberOfPoints() != 6)
  {
    std::cout << "Wrong number of point in the output" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
