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

#include "otbEigenvalueLikelihoodMaximisation.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingStatisticsVectorImageFilter.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;

typedef otb::EigenvalueLikelihoodMaximisation<double> ELMType;

int otbEigenvalueLikelihoodMaximizationTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  StreamingStatisticsVectorImageFilterType::Pointer stats = StreamingStatisticsVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  stats->SetInput(reader->GetOutput());
  stats->Update();

  ELMType::Pointer elm = ELMType::New();
  elm->SetCovariance(stats->GetCovariance().GetVnlMatrix());
  elm->SetCorrelation(stats->GetCorrelation().GetVnlMatrix());
  elm->SetNumberOfPixels(reader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels());
  elm->Compute();

  std::ofstream file;
  file.open(outfname);
  file.precision(5);
  file << "Number of Endmembers: " << elm->GetNumberOfEndmembers() << std::endl;
  file << "Likelihood: " << elm->GetLikelihood() << std::endl;
  file.close();

  std::cout << "Number of Endmembers: " << elm->GetNumberOfEndmembers() << std::endl;
  std::cout << "Likelihood: " << elm->GetLikelihood() << std::endl;

  return EXIT_SUCCESS;
}
