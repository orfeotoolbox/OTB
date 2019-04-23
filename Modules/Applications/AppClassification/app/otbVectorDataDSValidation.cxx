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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperStringListParameter.h"


namespace otb
{
namespace Wrapper
{

class VectorDataDSValidation: public Application
{


public:
   /** Standard class typedefs. */
  typedef VectorDataDSValidation Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;


  typedef double                          PrecisionType;
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                          VectorDataValidationFilterType;
  typedef VectorDataValidationFilterType::LabelSetType
                                          LabelSetType;
  typedef FuzzyDescriptorsModelManager::DescriptorsModelType DescriptorsModelType;

  typedef otb::Wrapper::StringListParameter::StringListType    StringListType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VectorDataDSValidation, otb::Application);

private:
  void DoInit() override
  {
    SetName("VectorDataDSValidation");
    SetDescription("Vector data validation based on the fusion of features using Dempster-Shafer evidence theory framework.");

    SetDocLongDescription("This application validates or unvalidate the studied samples using the Dempster-Shafer theory.");
    SetDocLimitations("None.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("http://en.wikipedia.org/wiki/Dempster-Shafer_theory");

    AddDocTag(Tags::FeatureExtraction);


    AddParameter(ParameterType_InputVectorData, "in", "Input Vector Data");
    SetParameterDescription("in", "Input vector data to validate");

    AddParameter(ParameterType_InputFilename, "descmod", "Descriptors model filename");
    SetParameterDescription("descmod", "Fuzzy descriptors model (xml file)");

    AddParameter(ParameterType_StringList, "belsup", "Belief Support");
    SetParameterDescription("belsup", "Dempster Shafer study hypothesis to compute belief");

    AddParameter(ParameterType_StringList, "plasup", "Plausibility Support");
    SetParameterDescription("plasup", "Dempster Shafer study hypothesis to compute plausibility");

    AddParameter(ParameterType_OutputVectorData, "out", "Output Vector Data");
    SetParameterDescription("out", "Output VectorData containing only the validated samples");

    AddParameter(ParameterType_String, "cri", "Criterion");
    SetParameterDescription("cri", "Dempster Shafer criterion (by default (belief+plausibility)/2)");
    MandatoryOff("cri");
    SetParameterString("cri", "((Belief + Plausibility)/2.)");

    AddParameter(ParameterType_Float, "thd", "Criterion threshold");
    SetParameterDescription("thd", "Criterion threshold (default 0.5)");
    MandatoryOff("thd");
    SetParameterFloat("thd",0.5);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp");
    SetDocExampleParameterValue("belsup", "cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp");
    SetDocExampleParameterValue("descmod", "DSFuzzyModel.xml");
    SetDocExampleParameterValue("out", "VectorDataDSValidation.shp");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent


    // .. //


  }

  void DoExecute() override
  {

    //Read the vector data
    VectorDataType::Pointer inVectorData = GetParameterVectorData("in");
    inVectorData->Update();

    // Load the descriptors model
    std::string descModFile = GetParameterString("descmod");
    DescriptorsModelType descMod = FuzzyDescriptorsModelManager::Read(descModFile);

    LabelSetType Bhyp, Phyp;
    int nbSet;

    StringListType stringList = GetParameterStringList("belsup");
    nbSet = stringList.size();

    for (int i = 0; i < nbSet; i++)
      {
      std::string str = stringList[i];
      Bhyp.insert(str);
      }
    stringList = GetParameterStringList("plasup");
    nbSet = stringList.size();
    for (int i = 0; i < nbSet; i++)
      {
      std::string str = stringList[i];
      Phyp.insert(str);
      }

    // Process
    m_ValidationFilter = VectorDataValidationFilterType::New();
    m_ValidationFilter->SetInput(inVectorData);
    m_ValidationFilter->SetDescriptorModels(descMod);
    m_ValidationFilter->SetBeliefHypothesis(Bhyp);
    m_ValidationFilter->SetPlausibilityHypothesis(Phyp);

    m_ValidationFilter->SetCriterionFormula(GetParameterString("cri"));
    m_ValidationFilter->SetCriterionThreshold(GetParameterFloat("thd"));

    // Set the output image
    SetParameterOutputVectorData("out", m_ValidationFilter->GetOutput());

  };

  VectorDataValidationFilterType::Pointer m_ValidationFilter;


};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataDSValidation);
