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
#include "otbExtern.h"

#include "itkMacro.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbVectorImageTo3DScalarImageFilter.h"

int otbVectorImageTo3DScalarImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int BiDimension  = 2;
  const unsigned int TriDimension  = 3;

  typedef double PixelType;

  typedef otb::VectorImage<PixelType, BiDimension> VectorImageType;
  typedef otb::Image<PixelType, TriDimension>      ImageType;

  typedef otb::VectorImageTo3DScalarImageFilter<VectorImageType, ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
