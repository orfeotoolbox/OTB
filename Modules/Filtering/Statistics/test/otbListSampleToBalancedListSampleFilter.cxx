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
#include "otbListSampleToBalancedListSampleFilter.h"
#include <fstream>

typedef itk::VariableLengthVector<double>             DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef itk::VariableLengthVector<unsigned int>        IntegerSampleType;
typedef itk::Statistics::ListSample<IntegerSampleType> IntegerSampleListType;

typedef itk::VariableLengthVector<float>             FloatSampleType;
typedef itk::Statistics::ListSample<FloatSampleType> FloatSampleListType;

typedef otb::Statistics::ListSampleToBalancedListSampleFilter
<FloatSampleListType, IntegerSampleListType, DoubleSampleListType> BalancingFilterType;


int otbListSampleToBalancedListSampleFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  BalancingFilterType::Pointer filter = BalancingFilterType::New();
  return EXIT_SUCCESS;
}

int otbListSampleToBalancedListSampleFilter(int argc, char * argv[])
{
  // Compute the number of samples
  const char * outfname = argv[1];
  unsigned int sampleSize = atoi(argv[2]);
  unsigned int nbSamples = (argc-3)/(sampleSize+1);  // +1 cause the
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

  ofs<<"Sample size: "<<sampleSize<<std::endl;
  ofs<<"Nb samples : "<<nbSamples<<std::endl;

  // InputSampleList and LabelSampleList
  for(unsigned int sampleId = 0; sampleId<nbSamples; ++sampleId)
    {
    for(unsigned int i = 0; i<sampleSize; ++i)
      {
      sample[i]=atof(argv[index]);
      ++index;
      }
    label[0]= atof(argv[index++]);

    ofs<<sample<<std::endl;
    ofs<<label<<std::endl;
    inputSampleList->PushBack(sample);
    labelSampleList->PushBack(label);
    }

  filter->Update();

  DoubleSampleListType::ConstIterator outIt = filter->GetOutput()->Begin();

  ofs<<"Output samples: "<<std::endl;

  while(outIt != filter->GetOutput()->End())
    {
    ofs<<outIt.GetMeasurementVector()<<std::endl;
    ++outIt;
    }

  IntegerSampleListType::ConstIterator labelIt = filter->GetOutputLabel()->Begin();
  ofs<<"Output Label samples: "<<std::endl;

  while(labelIt != filter->GetOutputLabel()->End())
    {
    ofs<<labelIt.GetMeasurementVector()<<std::endl;
    ++labelIt;
    }
  ofs.close();

  std::cout <<"Output balanced SampleList Size         : "<< filter->GetOutput()->Size()  << std::endl;
  std::cout <<"Output balanced Labeled SampleList Size : "<< filter->GetOutputLabel()->Size() << std::endl;

  return EXIT_SUCCESS;
}
