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


#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorData.h"
#include "otbVectorDataToMapFilter.h"

int otbVectorDataToMapFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef otb::VectorData<>             VectorDataType;
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

  typedef otb::VectorDataToMapFilter<VectorDataType, ImageType> VectorDataToMapFilterType;
  VectorDataToMapFilterType::Pointer vectorDataRendering = VectorDataToMapFilterType::New();

  std::cout << vectorDataRendering << std::endl;

  return EXIT_SUCCESS;
}
