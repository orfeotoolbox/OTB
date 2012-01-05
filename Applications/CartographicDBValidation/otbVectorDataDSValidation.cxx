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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperStringListParameter.h"

#include "otbVectorData.h"
#include "otbFuzzyDescriptorsModelManager.h"


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
  void DoInit()
  {
    SetName("VectorDataDSValidation");
    SetDescription("Vector data validation based on the fusion of features using Dempster-Shafer evidence theory framework.");

    SetDocName("Vector Data validation");
    SetDocLongDescription("Vector data validation based on the fusion of features using Dempster-Shafer evidence theory framework.");
    SetDocLimitations("None.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
   
    AddDocTag(Tags::FeatureExtraction);


    AddParameter(ParameterType_InputVectorData, "in", "Input Vector Data");
    SetParameterDescription("in", "Input vector data for validation");

    AddParameter(ParameterType_Filename, "descmod", "Descriptors model filename");
    SetParameterDescription("descmod", "Fuzzy descriptors model (xml file)");

    AddParameter(ParameterType_StringList, "belsup", "Belief Support");
    SetParameterDescription("belsup", "Dempster Shafer study hypothesis to compute belief");

    AddParameter(ParameterType_StringList, "plasup", "Plausibility Support");
    SetParameterDescription("plasup", "Dempster Shafer study hypothesis to compute plausibility");

    AddParameter(ParameterType_String, "cri", "Criterion");
    SetParameterDescription("cri", "Dempster Shafer criterion (by default (belief+plausibility)/2)");
    MandatoryOff("cri");
    SetParameterString("cri", "((Belief + Plausibility)/2.)");

    AddParameter(ParameterType_Float, "thd", "Criterion threshold");
    SetParameterDescription("thd", "Criterion threshold (default 0.5)");
    MandatoryOff("thd");
    SetParameterFloat("thd", 0.5);

    AddParameter(ParameterType_OutputVectorData, "out", "Output Vector Data");
    SetParameterDescription("out", "Output validated vector data");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp");
    SetDocExampleParameterValue("belsup", "cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp");
    SetDocExampleParameterValue("descmod", "DSFuzzyModel.xml");
    SetDocExampleParameterValue("out", "VectorDataDSValidation.xml");
    
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent


    // .. //


  }

  void DoExecute()
  {

    //Read the vector data
    VectorDataType::Pointer inVectorData = GetParameterVectorData("in");
    inVectorData->Update();

    // Load the descriptors model
    std::string descModFile = GetParameterString("descmod");
    DescriptorsModelType descMod = FuzzyDescriptorsModelManager::Read(descModFile.c_str());

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
