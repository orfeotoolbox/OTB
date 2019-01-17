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


#include "otbSimplePointCountStrategy.h"
#include "itkPointSet.h"
#include "otbImage.h"
#include "itkVariableLengthVector.h"

int otbSimplePointCountStrategyTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  const unsigned int Dimension = 2;
  typedef float RealType;

  typedef otb::Image<RealType, Dimension>     ImageType;
  typedef itk::VariableLengthVector<RealType> RealVectorType;

  typedef itk::PointSet<RealVectorType, Dimension> PointSetType;
  typedef PointSetType::PointsContainer            PointsContainerType;
  typedef ImageType::IndexType                     IndexType;

  typedef otb::Count<PointSetType, unsigned int, IndexType> counterType;

  /*pointSet de test*/
  PointSetType::Pointer                  pointset =  PointSetType::New();
  PointsContainerType::ElementIdentifier count = 0;
  PointSetType::PointType                Point;
  Point[0] = 12.14;
  Point[1] = 14.14;

  for (int i = 0; i < 10; ++i)
    {
    pointset->SetPoint(count, Point);
    count++;
    } /** Fin creation pointset de test */

  /*Test du filtre*/
  IndexType index;
  index[0] = 12;
  index[1] = 14;
  unsigned int rad = 2;
  counterType  comptemoica;

  std::cout << "Le resultat retourne est " << comptemoica(pointset, rad, index) << std::endl;

  return EXIT_SUCCESS;

}
