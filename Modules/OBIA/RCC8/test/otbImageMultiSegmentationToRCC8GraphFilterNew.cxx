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
#include "otbExtern.h"

#include "itkMacro.h"
#include "otbRCC8Graph.h"
#include "otbRCC8VertexBase.h"
#include "otbPolygon.h"
#include "otbImageMultiSegmentationToRCC8GraphFilter.h"

int otbImageMultiSegmentationToRCC8GraphFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned short                        LabelPixelType;
  typedef otb::Polygon<>                        PolygonType;
  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
  typedef otb::RCC8VertexBase<PolygonType>      VertexType;
  typedef otb::RCC8Graph<VertexType>            RCC8GraphType;
  typedef otb::ImageMultiSegmentationToRCC8GraphFilter<LabelImageType, RCC8GraphType>
  RCC8GraphFilterType;

  // Instantiation
  RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
