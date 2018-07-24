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



#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbApplyGainFilter.h"

int otbApplyGainFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double InputPixelType;
  typedef double OutputPixelType;
  typedef float LutPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image< InputPixelType ,  Dimension > InputImageType;
  typedef otb::Image< OutputPixelType , Dimension > OutputImageType;
  typedef otb::VectorImage< LutPixelType , Dimension > LutImageType;
  typedef otb::ApplyGainFilter
      < InputImageType , LutImageType , OutputImageType > FilterType;


  FilterType::Pointer appGain ( FilterType::New() );

  std::cout << appGain << std::endl;

  return EXIT_SUCCESS;
}