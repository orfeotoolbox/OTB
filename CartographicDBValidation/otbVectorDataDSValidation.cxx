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
  descriptor->SetName("Vector data validation");
  descriptor->SetDescription("Vector data validation using Classifier fusion.");
  descriptor->AddOption("InputShapeFileName", "Input Shape file name",
                        "in", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputShapeFileName", "Output Shape file name",
                        "out", 1, true, ApplicationDescriptor::FileName);

  descriptor->AddOption("DescriptorsModelFileName", "Fuzzy descriptors model xml file",
                        "descMod", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOptionNParams("BeliefSupport", "Dempster Shafer study hypothesis to compute Belief",
                               "BelSup", true, ApplicationDescriptor::StringList);
  descriptor->AddOptionNParams("PlausibilitySupport", "Dempster Shafer study hypothesis to compute Plausibility",
                               "PlaSup", true, ApplicationDescriptor::StringList);


  descriptor->AddOption("CriterionFormula", "Criterion formula expression (default: ((Belief + Plausibility)/2) >= 0.5)",
                        "Cri", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("CriterionThreshold", "Criterion threshold (by default 0.5)",
                        "thd", 1, false, ApplicationDescriptor::Real);
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
  LabelSetType Bhyp, Phyp;
  unsigned int nbSet = parseResult->GetNumberOfParameters("BeliefSupport");
  for (unsigned int i = 0; i < nbSet; i++)
    {
    Bhyp.insert(parseResult->GetParameterString("BeliefSupport", i));
    }
  nbSet = parseResult->GetNumberOfParameters("PlausibilitySupport");
  for (unsigned int i = 0; i < nbSet; i++)
    {
    Phyp.insert(parseResult->GetParameterString("PlausibilitySupport", i));
    }

  // Process
  VectorDataValidationFilterType::Pointer filter = VectorDataValidationFilterType::New();
  filter->SetInput(vdReader->GetOutput());
  filter->SetDescriptorModels(descMod);
  filter->SetBeliefHypothesis(Bhyp);
  filter->SetPlausibilityHypothesis(Phyp);
  if (parseResult->IsOptionPresent("CriterionFormula"))
    {
    filter->SetCriterionFormula(parseResult->GetParameterString("CriterionFormula"));
    }
  if (parseResult->IsOptionPresent("CriterionThreshold"))
    {
    filter->SetCriterionThreshold(parseResult->GetParameterDouble("CriterionThreshold"));
    }

  // Write the output
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  vdWriter->SetFileName(parseResult->GetParameterString("OutputShapeFileName"));
  vdWriter->SetInput(filter->GetOutput());
  vdWriter->Update();
 
  return EXIT_SUCCESS;
}
}
