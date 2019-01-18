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
#include "itkVariableLengthVector.h"
#include "otbShiftScaleSampleListFilter.h"

#include <fstream>

typedef itk::VariableLengthVector<double> DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef itk::VariableLengthVector<float> FloatSampleType;
typedef itk::Statistics::ListSample<FloatSampleType> FloatSampleListType;
typedef otb::Statistics::ShiftScaleSampleListFilter<FloatSampleListType, DoubleSampleListType> ShiftScaleFilterType;



int otbShiftScaleSampleListFilter(int argc, char * argv[])
{
 // Compute the number of samples
 const char * outfname = argv[1];
 unsigned int sampleSize = atoi(argv[2]);
 unsigned int nbSamples = (argc-2*sampleSize-2)/sampleSize;

 FloatSampleListType::Pointer inputSampleList = FloatSampleListType::New();
 inputSampleList->SetMeasurementVectorSize(sampleSize);

 ShiftScaleFilterType::Pointer filter = ShiftScaleFilterType::New();
 filter->SetInput(inputSampleList);

 FloatSampleType sample(sampleSize);

 unsigned int index = 3;

 std::ofstream ofs(outfname);

 ofs<<"Sample size: "<<sampleSize<<std::endl;
 ofs<<"Nb samples : "<<nbSamples<<std::endl;

 for(unsigned int i = 0; i<sampleSize; ++i)
 {
  sample[i]=atof(argv[index]);
  ++index;
 }

 ofs<<"Shifts: "<<sample<<std::endl;

 filter->SetShifts(sample);

 for(unsigned int i = 0; i<sampleSize; ++i)
 {
  sample[i]=atof(argv[index]);
  ++index;
 }

 ofs<<"Scales: "<<sample<<std::endl;

 filter->SetScales(sample);

 ofs<<"Input samples: "<<std::endl;

 for(unsigned int sampleId = 0; sampleId<nbSamples; ++sampleId)
 {
  for(unsigned int i = 0; i<sampleSize; ++i)
   {
    sample[i]=atof(argv[index]);
    ++index;
   }
  ofs<<sample<<std::endl;
  inputSampleList->PushBack(sample);
 }

 filter->Update();

 DoubleSampleListType::ConstIterator outIt = filter->GetOutput()->Begin();

 ofs<<"Output samples: "<<std::endl;

 while(outIt != filter->GetOutput()->End())
 {
  ofs<<outIt.GetMeasurementVector()<<std::endl;
  ++outIt;
 }

 ofs.close();

 return EXIT_SUCCESS;
}
