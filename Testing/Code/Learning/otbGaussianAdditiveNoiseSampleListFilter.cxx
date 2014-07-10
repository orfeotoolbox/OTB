/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "itkListSample.h"
#include "otbGaussianAdditiveNoiseSampleListFilter.h"
#include <fstream>

typedef itk::VariableLengthVector<double> DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef itk::VariableLengthVector<float> FloatSampleType;
typedef itk::Statistics::ListSample<FloatSampleType> FloatSampleListType;
typedef otb::Statistics::GaussianAdditiveNoiseSampleListFilter
<FloatSampleListType, DoubleSampleListType> GaussianFilterType;


int otbGaussianAdditiveNoiseSampleListFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  GaussianFilterType::Pointer filter = GaussianFilterType::New();
  return EXIT_SUCCESS;
}

int otbGaussianAdditiveNoiseSampleListFilter(int argc, char * argv[])
{
  // Compute the number of samples
  const char * outfname = argv[1];
  unsigned int sampleSize = atoi(argv[2]);
  unsigned int nbSamples = (argc-3)/sampleSize;

  FloatSampleListType::Pointer inputSampleList = FloatSampleListType::New();
  inputSampleList->SetMeasurementVectorSize(sampleSize);

  GaussianFilterType::Pointer filter = GaussianFilterType::New();
  filter->SetInput(inputSampleList);

  // Input Sample
  FloatSampleType sample(sampleSize);

  unsigned int index = 3;

  std::ofstream ofs(outfname);

  ofs<<"Sample size: "<<sampleSize<<std::endl;
  ofs<<"Nb samples : "<<nbSamples<<std::endl;

  // InputSampleList
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
