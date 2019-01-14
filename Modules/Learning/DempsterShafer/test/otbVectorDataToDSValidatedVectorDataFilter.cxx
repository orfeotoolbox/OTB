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



#include "otbVectorDataToDSValidatedVectorDataFilter.h"

#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"


int otbVectorDataToDSValidatedVectorDataFilter(int itkNotUsed(argc), char* argv[])
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
