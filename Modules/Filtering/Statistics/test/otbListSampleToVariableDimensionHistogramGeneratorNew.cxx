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

#include "otbVectorImage.h"
#include "itkListSample.h"
#include "otbListSampleToVariableDimensionHistogramGenerator.h"

int otbListSampleToVariableDimensionHistogramGeneratorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef double                                       PixelType;
  typedef otb::VectorImage<PixelType>                  VectorImageType;
  typedef VectorImageType::PixelType                   VectorPixelType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef otb::ListSampleToVariableDimensionHistogramGenerator
  <ListSampleType, PixelType>                         HistogramGeneratorType;

  // Instantiation
  HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();

  std::cout << generator << std::endl;

  return EXIT_SUCCESS;
}
