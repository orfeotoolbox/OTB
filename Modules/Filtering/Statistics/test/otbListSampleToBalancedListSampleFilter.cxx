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


#include "itkListSample.h"
#include "otbListSampleToBalancedListSampleFilter.h"
#include <fstream>

typedef itk::VariableLengthVector<double>             DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef itk::VariableLengthVector<unsigned int>        IntegerSampleType;
typedef itk::Statistics::ListSample<IntegerSampleType> IntegerSampleListType;

typedef itk::VariableLengthVector<float>             FloatSampleType;
typedef itk::Statistics::ListSample<FloatSampleType> FloatSampleListType;

typedef otb::Statistics::ListSampleToBalancedListSampleFilter<FloatSampleListType, IntegerSampleListType, DoubleSampleListType> BalancingFilterType;


int otbListSampleToBalancedListSampleFilter(int argc, char* argv[])
{
  // Compute the number of samples
  const char*  outfname   = argv[1];
  unsigned int sampleSize = atoi(argv[2]);
  unsigned int nbSamples  = (argc - 3) / (sampleSize + 1); // +1 cause the
                                                           // label is added
                                                           // in the commandline

  IntegerSampleListType::Pointer labelSampleList = IntegerSampleListType::New();
  labelSampleList->SetMeasurementVectorSize(1);

  FloatSampleListType::Pointer inputSampleList = FloatSampleListType::New();
  inputSampleList->SetMeasurementVectorSize(sampleSize);

  BalancingFilterType::Pointer filter = BalancingFilterType::New();
  filter->SetInput(inputSampleList);
  filter->SetInputLabel(labelSampleList);

  // Input Sample
  FloatSampleType   sample(sampleSize);
  IntegerSampleType label(1);

  unsigned int index = 3;

  std::ofstream ofs(outfname);

  ofs << "Sample size: " << sampleSize << std::endl;
  ofs << "Nb samples : " << nbSamples << std::endl;

  // InputSampleList and LabelSampleList
  for (unsigned int sampleId = 0; sampleId < nbSamples; ++sampleId)
  {
    for (unsigned int i = 0; i < sampleSize; ++i)
    {
      sample[i] = atof(argv[index]);
      ++index;
    }
    label[0] = atof(argv[index++]);

    ofs << sample << std::endl;
    ofs << label << std::endl;
    inputSampleList->PushBack(sample);
    labelSampleList->PushBack(label);
  }

  filter->Update();

  DoubleSampleListType::ConstIterator outIt = filter->GetOutput()->Begin();

  ofs << "Output samples: " << std::endl;

  while (outIt != filter->GetOutput()->End())
  {
    ofs << outIt.GetMeasurementVector() << std::endl;
    ++outIt;
  }

  IntegerSampleListType::ConstIterator labelIt = filter->GetOutputLabel()->Begin();
  ofs << "Output Label samples: " << std::endl;

  while (labelIt != filter->GetOutputLabel()->End())
  {
    ofs << labelIt.GetMeasurementVector() << std::endl;
    ++labelIt;
  }
  ofs.close();

  std::cout << "Output balanced SampleList Size         : " << filter->GetOutput()->Size() << std::endl;
  std::cout << "Output balanced Labeled SampleList Size : " << filter->GetOutputLabel()->Size() << std::endl;

  return EXIT_SUCCESS;
}
