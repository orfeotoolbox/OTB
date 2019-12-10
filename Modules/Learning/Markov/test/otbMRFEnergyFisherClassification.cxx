/*
 * Copyright (C) 2008 Jan Wegner
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


#include "otbMRFEnergyFisherClassification.h"
#include "otbImage.h"
#include <fstream>


int otbMRFEnergyFisherClassification(int itkNotUsed(argc), char* argv[])
{
  const char* outputFile = argv[1];

  typedef double PixelTypeInput;
  typedef int    PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2> ImageType;
  typedef otb::Image<PixelTypeLabel, 2> LabelType;

  typedef otb::MRFEnergyFisherClassification<ImageType, LabelType> MRFFisherType;

  MRFFisherType::Pointer classif = MRFFisherType::New();

  classif->SetNumberOfParameters(6);
  MRFFisherType::ParametersType param(6);
  param[0] = 0;
  param[1] = 1;
  param[2] = 2;
  param[3] = 3;
  param[4] = 4;
  param[5] = 5;
  classif->SetParameters(param);
  std::ofstream file;
  file.open(outputFile);

  file << std::endl;
  file << "Number of parameters: " << classif->GetNumberOfParameters() << std::endl;
  file << "GetSingleValue(10, 1): " << classif->GetSingleValue(10, 1) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
