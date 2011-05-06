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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbVectorDataToDSValidatedVectorDataFilter.h"
#include "otbMassOfBelief.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

int otbVectorDataToDSValidatedVectorDataFilterNew(int argc, char* argv[])
{
  typedef float                           PrecisionType;
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  
  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                          VectorDataValidationFilterType;
  /*
  VectorDataValidationFilterType::Pointer filter =
    VectorDataValidationFilterType::New();
  
  std::cout<<filter<<std::endl;
  */

  return EXIT_SUCCESS;
}

int otbVectorDataToDSValidatedVectorDataFilter(int argc, char* argv[])
{
  const char * inputVD  = argv[1];
  const char * outputVD = argv[2];

  typedef float                           PrecisionType;
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>
                                          VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>
                                          VectorDataWriterType;

  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                          VectorDataValidationFilterType;
  typedef VectorDataValidationFilterType::LabelSetType
                                          LabelSetType;
  
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();

  VectorDataValidationFilterType::Pointer filter =
    VectorDataValidationFilterType::New();
  
  vdReader->SetFileName(inputVD);
  vdReader->Update();

  filter->SetInput(vdReader->GetOutput());
  LabelSetType hypothesis;
  hypothesis.insert("NDVI");
  hypothesis.insert("RADIOM");
  //hypothesis.insert("LSD_");
  //hypothesis.insert("SHADOW_");
  filter->SetHypothesis(hypothesis);

  vdWriter->SetFileName(outputVD);
  vdWriter->SetInput(filter->GetOutput());
  vdWriter->Update();
  
  std::cout << "Input VectorData Size : "
            << vdReader->GetOutput()->Size() << std::endl
            << "CriterionFormula : "
            << filter->GetCriterionFormula() << std::endl
            //<< "Hypothesis : "
            //<< filter->GetHypothesis()
            << "Output VecttorData Size : "
            << filter->GetOutput()->Size()
            << std::endl;

  return EXIT_SUCCESS;
}
