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
#include "itkVariableLengthVector.h"
#include "otbConcatenateSampleListFilter.h"

#include <fstream>

typedef itk::VariableLengthVector<double> DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef otb::Statistics::ConcatenateSampleListFilter<DoubleSampleListType> ConcatenateFilterType;

int otbConcatenateSampleListFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
 ConcatenateFilterType::Pointer instance = ConcatenateFilterType::New();

 return EXIT_SUCCESS;
}

int otbConcatenateSampleListFilter(int itkNotUsed(argc), char * argv[])
{
 // Compute the number of samples
 const char * outfname = argv[1];
 unsigned int sampleSize = atoi(argv[2]);
 unsigned int nbSamples1 = atoi(argv[3]);
 unsigned int nbSamples2 = atoi(argv[4]);

 DoubleSampleListType::Pointer inputSampleList1 = DoubleSampleListType::New();
 inputSampleList1->SetMeasurementVectorSize(sampleSize);

 DoubleSampleListType::Pointer inputSampleList2 = DoubleSampleListType::New();
 inputSampleList2->SetMeasurementVectorSize(sampleSize);

 ConcatenateFilterType::Pointer filter = ConcatenateFilterType::New();
 filter->AddInput(inputSampleList1);
 filter->AddInput(inputSampleList2);

 DoubleSampleType sample(sampleSize);

 unsigned int index = 5;

 std::ofstream ofs(outfname);

 ofs<<"Sample size: "<<sampleSize<<std::endl;
 ofs<<"Nb samples 1: "<<nbSamples1<<std::endl;
 ofs<<"Nb samples 2: "<<nbSamples2<<std::endl;

 ofs<<"Input samples 1: "<<std::endl;

 for(unsigned int sampleId = 0; sampleId<nbSamples1; ++sampleId)
 {
  for(unsigned int i = 0; i<sampleSize; ++i)
   {
    sample[i]=atof(argv[index]);
    ++index;
   }
  ofs<<sample<<std::endl;
  inputSampleList1->PushBack(sample);
 }

 ofs<<"Input samples 2: "<<std::endl;

 for(unsigned int sampleId = 0; sampleId<nbSamples2; ++sampleId)
 {
  for(unsigned int i = 0; i<sampleSize; ++i)
  {
   sample[i]=atof(argv[index]);
   ++index;
  }
  ofs<<sample<<std::endl;
  inputSampleList2->PushBack(sample);
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
