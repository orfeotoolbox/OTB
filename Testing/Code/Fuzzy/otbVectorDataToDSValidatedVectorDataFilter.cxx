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


#include "otbVectorDataToDSValidatedVectorDataFilter.h"
#include "otbMassOfBelief.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbFuzzyDescriptorsModelManager.h"

int otbVectorDataToDSValidatedVectorDataFilterNew(int argc, char* argv[])
{
  typedef float                           PrecisionType;
  typedef otb::VectorData<PrecisionType>  VectorDataType;

  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                          VectorDataValidationFilterType;

  VectorDataValidationFilterType::Pointer filter =
    VectorDataValidationFilterType::New();

  std::cout << filter << std::endl;

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
  typedef otb::FuzzyDescriptorsModelManager
                                          FuzzyManagerType;
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();

  VectorDataValidationFilterType::Pointer filter =
    VectorDataValidationFilterType::New();

  vdReader->SetFileName(inputVD);
  vdReader->Update();

  filter->SetInput(vdReader->GetOutput());
  //filter->GetDescriptorModels();
  FuzzyManagerType::Print(filter->GetDescriptorModels());
  LabelSetType plau, bel;
  plau.insert("NONDVI");
  plau.insert("ROADSA");
  plau.insert("NOBUIL");
  filter->SetPlausibilityHypothesis(plau);
  bel.insert("ROADSA");
  bel.insert("NONDVI");
  bel.insert("NOBUIL");
  filter->SetBeliefHypothesis(bel);

  std::vector<double> stdModel;
  stdModel.push_back(0.25);
  stdModel.push_back(0.50);
  stdModel.push_back(0.75);
  stdModel.push_back(0.90);
  filter->AddDescriptor("NONDVI", stdModel);
  filter->AddDescriptor("ROADSA", stdModel);
  filter->AddDescriptor("NOBUIL", stdModel);

  FuzzyManagerType::Print(filter->GetDescriptorModels());

  vdWriter->SetFileName(outputVD);
  vdWriter->SetInput(filter->GetOutput());
  vdWriter->Update();

  std::cout << "Input VectorData Size : "
            << vdReader->GetOutput()->Size() << std::endl
            << "CriterionFormula : "
            << filter->GetCriterionFormula() << std::endl
            << "Output VecttorData Size : "
            << filter->GetOutput()->Size()
            << std::endl;

  return EXIT_SUCCESS;
}
