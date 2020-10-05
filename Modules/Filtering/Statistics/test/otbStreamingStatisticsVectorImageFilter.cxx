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

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include <fstream>
#include "otbStreamingTraits.h"

int otbStreamingStatisticsVectorImageFilter(int argc, char* argv[])
{
  const char* infname          = argv[1];
  const char* outfname         = argv[2];
  float       userIgnoredValue = 0.;
  if (argc > 3)
  {
    userIgnoredValue = atof(argv[3]);
  }

  const unsigned int Dimension = 2;
  typedef double     PixelType;

  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>                      ReaderType;
  typedef otb::StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;

  // Instantiating object
  StreamingStatisticsVectorImageFilterType::Pointer filter = StreamingStatisticsVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(10);
  filter->SetInput(reader->GetOutput());
  if (argc > 3)
  {
    filter->SetIgnoreUserDefinedValue(true);
    filter->SetUserIgnoredValue(userIgnoredValue);
  }
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file << "Minimum: " << filter->GetMinimum() << std::endl;
  file << "Maximum: " << filter->GetMaximum() << std::endl;
  file << std::fixed;
  file.precision(5);
  file << "Sum: " << filter->GetSum() << std::endl;
  file << "Mean: " << filter->GetMean() << std::endl;
  file << "Correlation: " << filter->GetCorrelation() << std::endl;
  file << "Covariance: " << filter->GetCovariance() << std::endl;
  file << "Component Mean: " << filter->GetComponentMean() << std::endl;
  file << "Component Correlation: " << filter->GetComponentCorrelation() << std::endl;
  file << "Component Covariance: " << filter->GetComponentCovariance() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
