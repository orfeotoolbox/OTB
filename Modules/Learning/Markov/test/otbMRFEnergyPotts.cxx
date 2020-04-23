/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbMRFEnergyPotts.h"
#include "otbImage.h"
#include <fstream>

int otbMRFEnergyPotts(int itkNotUsed(argc), char* argv[])
{
  const char* outputFile = argv[1];

  typedef double PixelType;
  typedef int    PixelType2;
  typedef otb::Image<PixelType, 2>  ImageType;
  typedef otb::Image<PixelType2, 2> LabelType;

  typedef otb::MRFEnergyPotts<ImageType, LabelType> MRFEnergyPottsType;
  typedef MRFEnergyPottsType::InputImagePixelType    InputImagePixelType;
  typedef MRFEnergyPottsType::LabelledImagePixelType LabelledImagePixelType;

  MRFEnergyPottsType::Pointer object = MRFEnergyPottsType::New();

  InputImagePixelType    inPix = 10;
  LabelledImagePixelType inLab = 5;

  std::ofstream file;
  file.open(outputFile);

  file << "GetValue(const InputImagePixelType = 10, const LabelledImagePixelType = 5): " << object->GetValue(inPix, inLab) << std::endl;
  inPix = 5;
  file << "GetValue(const InputImagePixelType = 5, const LabelledImagePixelType = 5) : " << object->GetValue(inPix, inLab) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
