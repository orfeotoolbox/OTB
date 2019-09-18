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


#include "otbMRFEnergyEdgeFidelity.h"
#include "otbImage.h"
#include <fstream>

int otbMRFEnergyEdgeFidelity(int itkNotUsed(argc), char* argv[])
{
  const char* outputFile = argv[1];

  typedef double PixelTypeInput;
  typedef int    PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2>                    ImageType;
  typedef otb::Image<PixelTypeLabel, 2>                    LabelType;
  typedef otb::MRFEnergyEdgeFidelity<ImageType, LabelType> MRFEnergyType;

  MRFEnergyType::Pointer object = MRFEnergyType::New();

  std::ofstream file;
  file.open(outputFile);

  file << "GetSimngleValue(10, 50): " << object->GetSingleValue(10, 50) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
