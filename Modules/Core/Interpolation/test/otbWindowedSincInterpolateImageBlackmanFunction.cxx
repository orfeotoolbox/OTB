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

#include "itkMacro.h"

#include "otbWindowedSincInterpolateImageBlackmanFunction.h"
#include "itkConstantBoundaryCondition.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

int otbWindowedSincInterpolateImageBlackmanFunction(int argc, char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  typedef otb::Image<double, 2> ImageType;
  typedef otb::WindowedSincInterpolateImageBlackmanFunction<ImageType> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType                        ContinuousIndexType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  // Instantiating objects
  InterpolatorType::Pointer interp = InterpolatorType::New();

  unsigned int i = 4;

  std::vector<ContinuousIndexType> indicesList;

  while (i < static_cast<unsigned int>(argc) && (i + 1) < static_cast<unsigned int>(argc))
  {
    ContinuousIndexType idx1;
    idx1[0] = atof(argv[i]);
    idx1[1] = atof(argv[i + 1]);
    indicesList.push_back(idx1);

    i += 2;
  }

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  interp->SetInputImage(reader->GetOutput());
  interp->SetRadius(atoi(argv[3]));
  interp->Initialize();

  std::ofstream file;
  file.open(outfname);

  file << "Blackman Window Function" << std::endl;
  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
  {
    file << (*it) << " -> " << interp->EvaluateAtContinuousIndex((*it)) << std::endl;
  }
  file << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
