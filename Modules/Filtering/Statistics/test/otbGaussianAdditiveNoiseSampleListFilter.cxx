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


#include "itkListSample.h"
#include "otbGaussianAdditiveNoiseSampleListFilter.h"
#include <fstream>

typedef itk::VariableLengthVector<double>             DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef itk::VariableLengthVector<float>             FloatSampleType;
typedef itk::Statistics::ListSample<FloatSampleType> FloatSampleListType;
typedef otb::Statistics::GaussianAdditiveNoiseSampleListFilter<FloatSampleListType, DoubleSampleListType> GaussianFilterType;


int otbGaussianAdditiveNoiseSampleListFilter(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cout << "Usage : test_driver output_filename sample_size samples... " << std::endl;
    return EXIT_FAILURE;
  }
  // Compute the number of samples
  const char*  outfname   = argv[1];
  unsigned int sampleSize = atoi(argv[2]);
  if (sampleSize == 0)
  {
    std::cerr << "Sample size is null!" << std::endl;
    return EXIT_FAILURE;
  }
  unsigned int nbSamples = (argc - 3) / sampleSize;

  FloatSampleListType::Pointer inputSampleList = FloatSampleListType::New();
  inputSampleList->SetMeasurementVectorSize(sampleSize);

  GaussianFilterType::Pointer filter = GaussianFilterType::New();
  filter->SetInput(inputSampleList);

  // Input Sample
  FloatSampleType sample(sampleSize);

  unsigned int index = 3;

  std::ofstream ofs(outfname);

  ofs << "Sample size: " << sampleSize << std::endl;
  ofs << "Nb samples : " << nbSamples << std::endl;

  // InputSampleList
  for (unsigned int sampleId = 0; sampleId < nbSamples; ++sampleId)
  {
    for (unsigned int i = 0; i < sampleSize; ++i)
    {
      sample[i] = atof(argv[index]);
      ++index;
    }
    ofs << sample << std::endl;
    inputSampleList->PushBack(sample);
  }

  filter->Update();

  DoubleSampleListType::ConstIterator outIt = filter->GetOutput()->Begin();

  ofs << "Output samples: " << std::endl;

  while (outIt != filter->GetOutput()->End())
  {
    ofs << outIt.GetMeasurementVector() << std::endl;
    ++outIt;
  }

  ofs.close();

  return EXIT_SUCCESS;
}
