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

#include "itkMacro.h"
#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMActivationBuilder.h"
#include "itkListSample.h"
#include "otbImage.h"

int otbSOMActivationBuilderNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef float                                         ComponentType;
  typedef unsigned char                                 OutputPixelType;
  typedef itk::VariableLengthVector<ComponentType>      PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType> DistanceType;

  typedef otb::SOMMap<PixelType, DistanceType, Dimension>                     MapType;

  typedef itk::Statistics::ListSample<PixelType>                              ListSampleType;
  typedef otb::Image<OutputPixelType, Dimension>                              OutputImageType;
  typedef otb::SOMActivationBuilder<ListSampleType, MapType, OutputImageType> SOMActivationBuilderType;

  // Instantiation
  SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();

  std::cout << somAct << std::endl;

  return EXIT_SUCCESS;
}
