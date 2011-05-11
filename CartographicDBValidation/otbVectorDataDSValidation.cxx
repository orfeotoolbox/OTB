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

#include "otbVectorDataDSValidation.h"

#include "otbVectorDataToDSValidatedVectorDataFilter.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbFuzzyDescriptorsModelManager.h"

namespace otb
{
int VectorDataDSValidation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Vector data validation parser function");
  descriptor->SetDescription("Vector data validation.");
  descriptor->AddOption("InputShapeFileName", "Input Shape file name", "in", 1, true,
                        ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputShapeFileName", "Output Shape file name", "out", 1, true,
                        ApplicationDescriptor::FileName);
  descriptor->AddOptionNParams("Hypothesis", "Hypothesis (default: NDVI, RADIOM)",
                               "hyp", false, ApplicationDescriptor::StringList);
  descriptor->AddOption("DescriptorsModelFileName", "Fuzzy descriptors model xml file (default: NDVI(0.25, 0.5, 0.75, 0.99) / RADIOM(0.25, 0.5, 0.75, 0.90))", "descMod", 1, false,
                        ApplicationDescriptor::FileName);
  descriptor->AddOption("CriterionFormula", "Criterion formula expression (default: ((Belief + Plausibility)/2) >= 0.5)", "exp", 1, false,
                        ApplicationDescriptor::FileName);

  return EXIT_SUCCESS;
}


int VectorDataDSValidation::Execute(otb::ApplicationOptionsResult* parseResult)
{
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
  
  typedef FuzzyDescriptorsModelManager::DescriptorsModelType DescriptorsModelType;
 
  // Read the vector data
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(parseResult->GetParameterString("InputShapeFileName"));
  vdReader->Update();
 
  // Load the descriptors model
  std::string descModFile = parseResult->GetParameterString("DescriptorsModelFileName");
  DescriptorsModelType descMod = FuzzyDescriptorsModelManager::Read( descModFile.c_str() );

  // Load the hypothesis
  LabelSetType hypothesis;
  unsigned int nbOfHypo = parseResult->GetNumberOfParameters("Hypothesis");
  for (unsigned int i = 0; i < nbOfHypo; i++)
    {
      hypothesis.insert(parseResult->GetParameterString("Hypothesis", i));
    }
 
  // Process
  VectorDataValidationFilterType::Pointer filter = VectorDataValidationFilterType::New();
  filter->SetInput(vdReader->GetOutput());
  filter->SetDescriptorModels(descMod);
  filter->SetHypothesis(hypothesis);
  filter->SetCriterionFormula(parseResult->GetParameterString("CriterionFormula"));

  // Write the output
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  vdWriter->SetFileName(parseResult->GetParameterString("OutputShapeFileName"));
  vdWriter->SetInput(filter->GetOutput());
  vdWriter->Update();
 
  return EXIT_SUCCESS;
}
}
