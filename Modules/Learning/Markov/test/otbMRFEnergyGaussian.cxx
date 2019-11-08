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


#include "otbMRFEnergyGaussian.h"
#include "otbImage.h"
#include <fstream>

int otbMRFEnergyGaussian(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef double PixelType;
  typedef int    PixelType2;
  typedef otb::Image<PixelType, 2>  ImageType;
  typedef otb::Image<PixelType2, 2> LabelType;

  typedef otb::MRFEnergyGaussian<ImageType, LabelType> MRFEnergyGaussianType;
  typedef MRFEnergyGaussianType::InputImagePixelType    InputImagePixelType;
  typedef MRFEnergyGaussianType::LabelledImagePixelType LabelledImagePixelType;

  MRFEnergyGaussianType::Pointer object = MRFEnergyGaussianType::New();

  InputImagePixelType    inPix = 10;
  LabelledImagePixelType inLab = 5;

  if (object->GetSingleValue(inPix, inLab) != 25)
  {
    return EXIT_FAILURE;
  }

  inPix = 5;
  inLab = 10;
  if (object->GetSingleValue(inPix, inLab) != 25)
  {
    return EXIT_FAILURE;
  }

  inPix = 0;
  inLab = 0;
  if (object->GetSingleValue(inPix, inLab) != 0)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
