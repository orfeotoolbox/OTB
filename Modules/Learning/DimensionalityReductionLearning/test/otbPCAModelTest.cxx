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

#include "otbPCAModel.h"
#include "otbReadDataFile.h"

typedef otb::PCAModel<double>              PCAModelType;
typedef PCAModelType::InputListSampleType  InputListSampleType;
typedef PCAModelType::TargetListSampleType TargetListSampleType;


int otbPCAModelCanRead(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <model>" << std::endl;
    return EXIT_FAILURE;
  }

  PCAModelType::Pointer model = PCAModelType::New();
  std::string           filename(argv[1]);
  if (!model->CanReadFile(filename))
  {
    std::cerr << "Failed to read model file : " << filename << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int otbPCAModeTrain(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " letter.scale  model.out" << std::endl;
    return EXIT_FAILURE;
  }

  // Extract data from letter.scale
  InputListSampleType::Pointer  samples = InputListSampleType::New();
  TargetListSampleType::Pointer target  = TargetListSampleType::New();
  if (!otb::ReadDataFile(argv[1], samples, target))
  {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  PCAModelType::Pointer model = PCAModelType::New();
  model->SetDimension(14);
  model->SetWriteEigenvectors(true);
  model->SetInputListSample(samples);
  model->Train();
  model->Save(std::string(argv[2]));

  return EXIT_SUCCESS;
}
