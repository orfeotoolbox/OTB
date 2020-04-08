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

#include "itkMacro.h"

#include "otbBSplineInterpolateImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbBSplineInterpolateImageFunction(int argc, char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  typedef otb::Image<double, 2> ImageType;
  typedef otb::BSplineInterpolateImageFunction<ImageType, double, double> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;

  int i = 3;

  std::vector<ContinuousIndexType> indicesList;

  while (i < argc && (i + 1) < argc)
  {
    ContinuousIndexType idx;

    idx[0] = atof(argv[i]);
    idx[1] = atof(argv[i + 1]);

    indicesList.push_back(idx);

    i += 2;
  }

  // Instantiating object
  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();

  interpolator->SetInputImage(reader->GetOutput());

  std::ofstream file;
  file.open(outfname);

  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
  {
    file << (*it) << " -> " << interpolator->EvaluateAtContinuousIndex((*it)) << std::endl;
  }

  file.close();

  return EXIT_SUCCESS;
}
