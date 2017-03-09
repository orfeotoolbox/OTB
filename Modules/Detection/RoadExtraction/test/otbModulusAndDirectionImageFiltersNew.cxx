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
#include "otbImage.h"
#include <iostream>

#include "otbImageToModulusAndDirectionImageFilter.h"
#include "otbModulusAndDirectionImageToImageFilter.h"

int otbModulusAndDirectionImageFiltersNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned char InputPixelType;
  typedef double        OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> InputImageDirectionType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageDirectionType;

  typedef otb::ImageToModulusAndDirectionImageFilter<InputImageType,
      OutputImageType>          FilterType;
  typedef otb::ImageToModulusAndDirectionImageFilter<InputImageType, OutputImageType,
      OutputImageDirectionType> Filter2Type;

  FilterType::Pointer  filter = FilterType::New();
  Filter2Type::Pointer filter2 = Filter2Type::New();

  typedef otb::ModulusAndDirectionImageToImageFilter<InputImageType, InputImageDirectionType,
      OutputImageType> Filter3Type;
  Filter3Type::Pointer filter3 = Filter3Type::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
