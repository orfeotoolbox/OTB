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
  descriptor->SetDescription("Vector data validation using Clasifier fusion.");
  descriptor->AddOption("InputShapeFileName", "Input Shape file name",
                        "in", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputShapeFileName", "Output Shape file name",
                        "out", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("DescriptorsModelFileName", "Fuzzy descriptors model xml file (default: NONDVI(0.25, 0.5, 0.75, 0.99) / ROADSA(0.25, 0.5, 0.75, 0.90))",
                        "descMod", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOptionNParams("BeliefHypothesis", "Hypothesis (default: ROADSA) used to compute the Belief",
                               "Bhyp", false, ApplicationDescriptor::StringList);
  descriptor->AddOptionNParams("PlausibilityHypothesis", "Hypothesis (default: ROADSA, NONDVI, NOBUIL) used to compute the Plausibility",
                               "Phyp", false, ApplicationDescriptor::StringList);
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
  if (parseResult->IsOptionPresent("BeliefHypothesis"))
    {
    unsigned int nbOfHypo = parseResult->GetNumberOfParameters("BeliefHypothesis");
    for (unsigned int i = 0; i < nbOfHypo; i++)
      {
      Bhyp.insert(parseResult->GetParameterString("BeliefHypothesis", i));
      }
    }
  else
    {
    Bhyp.insert("ROADSA");
    }
  if (parseResult->IsOptionPresent("PlausibilityHypothesis"))
    {
    int nbSet = parseResult->GetNumberOfParameters("PlausibilityHypothesis");
    for (int i = 0; i < nbSet; i++)
      {
      Phyp.insert(parseResult->GetParameterString("PlausibilityHypothesis", i));
      }
    }
  else
    {
    Phyp.insert("ROADSA");
    Phyp.insert("NONDVI");
    Phyp.insert("NOBUIL");
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
