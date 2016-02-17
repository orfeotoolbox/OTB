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

#include <iostream>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperStringListParameter.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorDataToRandomLineGenerator.h"
#include "itkAmoebaOptimizer.h"
#include "otbStandardDSCostFunction.h"


namespace otb
{

namespace Wrapper
{

#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command
{
public:
typedef  CommandIterationUpdate   Self;
typedef  itk::Command             Superclass;
typedef itk::SmartPointer<Self>   Pointer;
itkNewMacro( Self );
protected:
CommandIterationUpdate() {};
public:
typedef itk::AmoebaOptimizer         OptimizerType;
typedef   const OptimizerType   *    OptimizerPointer;


void Execute(itk::Object *caller, const itk::EventObject & event)
{
  Execute( (const itk::Object *)caller, event);
}

void Execute(const itk::Object * object, const itk::EventObject & event)
{
  OptimizerPointer optimizer =
      dynamic_cast< OptimizerPointer >( object );
  if( ! itk::IterationEvent().CheckEvent( &event ) )
    {
    return;
    }
  std::ostringstream message;
  message << optimizer->GetCachedValue() << "   ";
  message << optimizer->GetCachedCurrentPosition() << std::endl;
  std::cout<<message.str()<<std::endl;
}


};


class DSFuzzyModelEstimation: public Application
{
public:
  /** Standard class typedefs. */
  typedef DSFuzzyModelEstimation Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef VectorData<double>                          VectorDataType;

  typedef VectorDataType::DataTreeType                DataTreeType;
  typedef VectorDataType::DataNodeType                DataNodeType;

  typedef VectorDataType::ValuePrecisionType          PrecisionType;
  typedef VectorDataType::PrecisionType               CoordRepType;

  typedef otb::Wrapper::StringListParameter::StringListType         StringListType;

  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                                            ValidationFilterType;
  typedef otb::StandardDSCostFunction<ValidationFilterType> CostFunctionType;
  typedef CostFunctionType::LabelSetType                    LabelSetType;

  typedef itk::AmoebaOptimizer                            OptimizerType;

  typedef otb::FuzzyDescriptorsModelManager::DescriptorsModelType
                                                          DescriptorsModelType;
  typedef otb::FuzzyDescriptorsModelManager::DescriptorListType
                                                          DescriptorListType;

     typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType>
     TreeIteratorType;


  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(DSFuzzyModelEstimation, otb::Application);

private:
  void DoInit()
  {
    SetName("DSFuzzyModelEstimation");
    SetDescription("Estimate feature fuzzy model parameters using 2 vector data (ground truth samples and wrong samples).");

    SetDocName("Fuzzy Model estimation");
    SetDocLongDescription("Estimate feature fuzzy model parameters using 2 vector data (ground truth samples and wrong samples).");
    SetDocLimitations("None.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputVectorData, "psin", "Input Positive Vector Data");
    SetParameterDescription("psin", "Ground truth vector data for positive samples");

    AddParameter(ParameterType_InputVectorData, "nsin", "Input Negative Vector Data");
    SetParameterDescription("nsin", "Ground truth vector data for negative samples");

    AddParameter(ParameterType_StringList, "belsup", "Belief Support");
    SetParameterDescription("belsup", "Dempster Shafer study hypothesis to compute belief");

    AddParameter(ParameterType_StringList, "plasup", "Plausibility Support");
    SetParameterDescription("plasup", "Dempster Shafer study hypothesis to compute plausibility");

    AddParameter(ParameterType_String, "cri", "Criterion");
    SetParameterDescription("cri", "Dempster Shafer criterion (by default (belief+plausibility)/2)");
    MandatoryOff("cri");
    SetParameterString("cri","((Belief + Plausibility)/2.)");

    AddParameter(ParameterType_Float,"wgt","Weighting");
    SetParameterDescription("wgt","Coefficient between 0 and 1 to promote undetection or false detections (default 0.5)");
    MandatoryOff("wgt");
    SetParameterFloat("wgt", 0.5);

    AddParameter(ParameterType_InputFilename,"initmod","initialization model");
    SetParameterDescription("initmod","Initialization model (xml file) to be used. If the xml initialization model is set, the descriptor list is not used (specified using the option -desclist)");
    MandatoryOff("initmod");

    AddParameter(ParameterType_StringList, "desclist","Descriptor list");
    SetParameterDescription("desclist","List of the descriptors to be used in the model (must be specified to perform an automatic initialization)");
    MandatoryOff("desclist");
    SetParameterString("desclist","");

    AddParameter(ParameterType_Int,"maxnbit","Maximum number of iterations");
    MandatoryOff("maxnbit");
    SetParameterDescription("maxnbit","Maximum number of optimizer iteration (default 200)");
    SetParameterInt("maxnbit", 200);

    AddParameter(ParameterType_Empty,"optobs","Optimizer Observer");
    SetParameterDescription("optobs","Activate the optimizer observer");
    MandatoryOff("optobs");

    AddParameter(ParameterType_OutputFilename,"out","Output filename");
    SetParameterDescription("out","Output model file name (xml file) contains the optimal model to perform information fusion.");

    // Doc example parameter settings
    SetDocExampleParameterValue("psin", "cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp");
    SetDocExampleParameterValue("nsin", "cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp");
    SetDocExampleParameterValue("belsup", "\"ROADSA\"");
    SetDocExampleParameterValue("plasup", "\"NONDVI\" \"ROADSA\" \"NOBUIL\"");
    SetDocExampleParameterValue("initmod", "Dempster-Shafer/DSFuzzyModel_Init.xml");
    SetDocExampleParameterValue("maxnbit", "4");
    SetDocExampleParameterValue("optobs", "true");
    SetDocExampleParameterValue("out", "DSFuzzyModelEstimation.xml");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent


    // .. //


  }

  void DoExecute()
  {

    //Instantiate
    m_CostFunction = CostFunctionType::New();
    m_Optimizer = OptimizerType::New();

    //Read the vector datas
    VectorDataType::Pointer psVectorData = GetParameterVectorData("psin");
    psVectorData->Update();
    VectorDataType::Pointer nsVectorData = GetParameterVectorData("nsin");
    nsVectorData->Update();

    // Load the initial descriptor model
    DescriptorListType descList;
    DescriptorsModelType descMod;
    if (IsParameterEnabled("initmod"))
      {
      std::string descModFile = GetParameterString("initmod");
      descMod = FuzzyDescriptorsModelManager::Read(descModFile.c_str());
      descList = FuzzyDescriptorsModelManager::GetDescriptorList(descMod);
      }
    else
      {
      StringListType stringList = GetParameterStringList("desclist");
      int nbsdDesc = stringList.size();
      for (int i = 0; i < nbsdDesc; i++)
        {
        descList.push_back(stringList[i]);
        }
      }

    m_CostFunction->SetDescriptorList(descList);

    // Compute statistics of all the descriptors

    std::vector<double> accFirstOrderPS, accSecondOrderPS, minPS, maxPS;
    accFirstOrderPS.resize(descList.size());
    accSecondOrderPS.resize(descList.size());
    std::fill(accFirstOrderPS.begin(), accFirstOrderPS.end(), 0);
    std::fill(accSecondOrderPS.begin(), accSecondOrderPS.end(), 0);
    minPS.resize(descList.size());
    maxPS.resize(descList.size());
    unsigned int accNbElemPS = 0;

    TreeIteratorType itVectorPS(psVectorData->GetDataTree());
    for (itVectorPS.GoToBegin(); !itVectorPS.IsAtEnd(); ++itVectorPS)
      {
      if (!itVectorPS.Get()->IsRoot() && !itVectorPS.Get()->IsDocument() && !itVectorPS.Get()->IsFolder())
        {
        DataNodeType::Pointer currentGeometry = itVectorPS.Get();

        for (unsigned int i = 0; i < descList.size(); ++i)
          {
          double desc = currentGeometry->GetFieldAsDouble(descList[i]);

          accFirstOrderPS[i] += desc;
          accSecondOrderPS[i] += desc * desc;

          if (desc < minPS[i])
            {
            minPS[i] = desc;
            }
          if (desc > maxPS[i])
            {
            maxPS[i] = desc;
            }

          }
        accNbElemPS++;
        }
      }

    TreeIteratorType itVectorNS(nsVectorData->GetDataTree());
    std::vector<double> accFirstOrderNS, accSecondOrderNS, minNS, maxNS;
    minNS.resize(descList.size());
    maxNS.resize(descList.size());
    accFirstOrderNS.resize(descList.size());
    accSecondOrderNS.resize(descList.size());
    std::fill(accFirstOrderNS.begin(), accFirstOrderNS.end(), 0);
    std::fill(accSecondOrderNS.begin(), accSecondOrderNS.end(), 0);
    std::fill(minNS.begin(), minNS.end(), 1);
    std::fill(maxNS.begin(), maxNS.end(), 0);
    unsigned int accNbElemNS = 0;

    for (itVectorNS.GoToBegin(); !itVectorNS.IsAtEnd(); ++itVectorNS)
      {
      if (!itVectorNS.Get()->IsRoot() && !itVectorNS.Get()->IsDocument() && !itVectorNS.Get()->IsFolder())
        {
        DataNodeType::Pointer currentGeometry = itVectorNS.Get();

        for (unsigned int i = 0; i < descList.size(); ++i)
          {
          double desc = currentGeometry->GetFieldAsDouble(descList[i]);

          accFirstOrderNS[i] += desc;
          accSecondOrderNS[i] += desc * desc;

          if (desc < minNS[i])
            {
            minNS[i] = desc;
            }
          if (desc > maxNS[i])
            {
            maxNS[i] = desc;
            }

          }
        accNbElemNS++;
        }
      }
    otbAppLogINFO( << "Descriptors Stats : ");
    otbAppLogINFO( << "Positive Samples");
    for (unsigned int i = 0; i < descList.size(); ++i)
      {
      double mean = accFirstOrderPS[i] / accNbElemPS;
      double stddev = vcl_sqrt(accSecondOrderPS[i] / accNbElemPS - mean * mean);
      otbAppLogINFO( << descList[i] << "  :  " << mean << " +/- " << stddev << "  (min: " << minPS[i] << "  max: " << maxPS[i] << ")"<< std::endl);
      }

    otbAppLogINFO( << "Negative Samples" << std::endl);
    for (unsigned int i = 0; i < descList.size(); ++i)
      {
      double mean = accFirstOrderNS[i] / accNbElemNS;
      double stddev = vcl_sqrt(accSecondOrderNS[i] / accNbElemNS - mean * mean);
      otbAppLogINFO(<< descList[i] << "  :  " << mean << " +/- " << stddev << "  (min: " << minNS[i] << "  max: " << maxNS[i] << ")"<< std::endl);
      }

    OptimizerType::ParametersType initialPosition(4 * descList.size());

    if (IsParameterEnabled("initmod"))
      {

      for (unsigned int i = 0; i < 4; i++)
        {
        for (unsigned int j = 0; j < descList.size(); j++)
          {
          initialPosition.SetElement(
                                     i + 4 * j,
                                     otb::FuzzyDescriptorsModelManager::GetDescriptor(descList[j].c_str(), descMod).second[i]);
          }
        }
      }
    else
      {
      for (unsigned int j = 0; j < descList.size(); j++)
        {
        initialPosition.SetElement((j * 4), std::min(minNS[j], maxPS[j]));
        initialPosition.SetElement((j * 4) + 2, std::max(minNS[j], maxPS[j]));
        initialPosition.SetElement(
                                   (j * 4) + 1,
                                       0.5
                                       * (initialPosition.GetElement((j * 4)) + initialPosition.GetElement((j * 4) + 2)));
        initialPosition.SetElement((j * 4) + 3, 0.95);
        }
      }

    otbAppLogINFO(<<"Initial model: "<<initialPosition);

    //Cost Function
    //Format Hypothesis
    LabelSetType Bhyp, Phyp;
    int nbSet;

    StringListType stringList = GetParameterStringList("belsup");
    nbSet = stringList.size();

    for (int i = 0; i < nbSet; i++)
      {
      std::string str = stringList[i];
      Bhyp.insert(str);
      }
    m_CostFunction->SetBeliefHypothesis(Bhyp);
    stringList = GetParameterStringList("plasup");
    nbSet = stringList.size();
    for (int i = 0; i < nbSet; i++)
      {
      std::string str = stringList[i];
      Phyp.insert(str);
      }
    m_CostFunction->SetPlausibilityHypothesis(Phyp);

    m_CostFunction->SetWeight(GetParameterFloat("wgt"));

    m_CostFunction->SetCriterionFormula(GetParameterString("cri"));

    m_CostFunction->SetGTVectorData(psVectorData);
    m_CostFunction->SetNSVectorData(nsVectorData);
    //Optimizer
    m_Optimizer->SetCostFunction(m_CostFunction);

    m_Optimizer->SetMaximumNumberOfIterations(GetParameterInt("maxnbit"));

    OptimizerType::ParametersType simplexDelta(m_CostFunction->GetNumberOfParameters());
    simplexDelta.Fill(0.1);

    m_Optimizer->AutomaticInitialSimplexOff();
    m_Optimizer->SetInitialSimplexDelta(simplexDelta);

    m_Optimizer->SetInitialPosition(initialPosition);

    // Create the Command observer and register it with the optimizer.
    CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
    if (IsParameterEnabled("optobs"))
      {
      m_Optimizer->AddObserver(itk::IterationEvent(), observer);
      }

    try
      {
      // do the optimization
      m_Optimizer->StartOptimization();
      }
    catch (itk::ExceptionObject& err)
      {
      // An error has occurred in the optimization.
      // Update the parameters
      otbAppLogFATAL("ERROR: Exception Caught!" << std::endl);
      otbAppLogFATAL(<< err.GetDescription() << std::endl);
      const unsigned int numberOfIterations = m_Optimizer->GetOptimizer()->get_num_evaluations();
      otbAppLogFATAL("numberOfIterations : " << numberOfIterations << std::endl);
      otbAppLogFATAL("Results : " << m_Optimizer->GetCurrentPosition() << std::endl);
      }
    // get the results
    const unsigned int numberOfIterations = m_Optimizer->GetOptimizer()->get_num_evaluations();
    otbAppLogINFO("numberOfIterations : " << numberOfIterations << std::endl);
    otbAppLogINFO("Results : " << m_Optimizer->GetCurrentPosition() << std::endl);

    for (unsigned int i = 0; i < descList.size(); i++)
      {
      otb::FuzzyDescriptorsModelManager::ParameterType tmpParams;
      for (unsigned int j = 0; j < 4; j++)
        {
        tmpParams.push_back(m_Optimizer->GetCurrentPosition()[(i * 4) + j]);
        }
      otb::FuzzyDescriptorsModelManager::AddDescriptor(descList[i], tmpParams, m_Model);
      }
    otb::FuzzyDescriptorsModelManager::Save(GetParameterString("out"), m_Model);

  };

  CostFunctionType::Pointer                               m_CostFunction;
  OptimizerType::Pointer                                  m_Optimizer;
  otb::FuzzyDescriptorsModelManager::DescriptorsModelType m_Model;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::DSFuzzyModelEstimation)

