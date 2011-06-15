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
#include "otbDSFuzzyModelEstimation.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorDataToRandomLineGenerator.h"
#include "itkAmoebaOptimizer.h"
#include "otbVectorDataToDSValidatedVectorDataFilter.h"
#include "otbStandardDSCostFunction.h"
#include "otbFuzzyDescriptorsModelManager.h"


//  The following piece of code implements an observer
//  that will monitor the evolution of the registration process.
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
  std::cout << optimizer->GetCachedValue() << "   ";
  std::cout << optimizer->GetCachedCurrentPosition() << std::endl;
}
};


//namespace otb
//{

int otb::DSFuzzyModelEstimation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("DSFuzzyModelEstimation");
  descriptor->SetDescription("Estimate feature fuzzy model parameters using 2 VectorDatas (ground thruth / wrong samples)");
  descriptor->AddOption("InputPositiveVectorData", "Ground Truth Vector Data",
                        "psin", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("InputNegativeVectorData", "Negative samples Vector Data",
                        "nsin", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOptionNParams("BeliefSupport", "Dempster Shafer study hypothesis to compute Belief",
                               "BelSup", true, ApplicationDescriptor::StringList);
  descriptor->AddOptionNParams("PlausibilitySupport", "Dempster Shafer study hypothesis to compute Plausibility",
                               "PlaSup", true, ApplicationDescriptor::StringList);
  descriptor->AddOption("Criterion", "Dempster Shafer Criterion (by default (Belief+Plausibility)/2)",
                        "cri", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("Weighting", "Coefficient between 0 and 1 to promote undetection or false detections (default 0.5)",
                        "wgt", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("InitModel", "Initial state for the model Optimizer",
                        "InitMod", 1, false, ApplicationDescriptor::FileName);
  descriptor->AddOptionNParams("DescriptorList", "List of the descriptors used in the model (must be specified to perform an automatic initialization)",
                        "DescList", false, ApplicationDescriptor::StringList);
  descriptor->AddOption("MaximumNumberOfIterations", "Maximum Number of Optimizer Iteration (default 200)",
                        "MaxNbIt", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("OptimizerObserver", "Activate or not the optimizer observer",
                        "OptObs", 0, false, ApplicationDescriptor::Boolean);
  descriptor->AddOption("Output", "Output Model File Name",
                        "out", 1,  true, ApplicationDescriptor::FileName);

  return EXIT_SUCCESS;
}


int otb::DSFuzzyModelEstimation::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorData<double>                     VectorDataType;
  typedef VectorDataType::ValuePrecisionType          PrecisionType;
  typedef VectorDataType::PrecisionType               CoordRepType;
  typedef otb::VectorDataFileReader<VectorDataType>   VectorDataReaderType;

  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                                      ValidationFilterType;
  typedef otb::StandardDSCostFunction<ValidationFilterType> CostFunctionType;
  typedef CostFunctionType::LabelSetType              LabelSetType;

  typedef itk::AmoebaOptimizer                        OptimizerType;

  typedef otb::FuzzyDescriptorsModelManager::DescriptorsModelType
                                                      DescriptorsModelType;
  typedef otb::FuzzyDescriptorsModelManager::DescriptorListType
                                                      DescriptorListType;
  //Instantiate
  VectorDataReaderType::Pointer               psReader = VectorDataReaderType::New();
  VectorDataReaderType::Pointer               nsReader = VectorDataReaderType::New();
  CostFunctionType::Pointer                   costFunction = CostFunctionType::New();
  OptimizerType::Pointer                      optimizer = OptimizerType::New();

  if ((parseResult->IsOptionPresent("DescriptorList") && parseResult->IsOptionPresent("InitModel"))
      || (!parseResult->IsOptionPresent("DescriptorList") && !parseResult->IsOptionPresent("InitModel")))
    {
    std::cout << "ERROR: An Initial Model OR a Descriptor List must be specified (but not both)" << std::endl;
    return EXIT_FAILURE;
    }

  //Read the vector datas
  psReader->SetFileName(parseResult->GetParameterString("InputPositiveVectorData"));
  psReader->Update();

  nsReader->SetFileName(parseResult->GetParameterString("InputNegativeVectorData"));
  nsReader->Update();

  // Load the initial descriptor model
  DescriptorListType   descList;
  DescriptorsModelType descMod;
  if (parseResult->IsOptionPresent("InitModel"))
    {
    std::string descModFile = parseResult->GetParameterString("InitModel");
    descMod  = FuzzyDescriptorsModelManager::Read( descModFile.c_str() );
    descList = FuzzyDescriptorsModelManager::GetDescriptorList(descMod);
    }
  else
    {
    int nbsdDesc = parseResult->GetNumberOfParameters("DescriptorList");
    for (int i = 0; i < nbsdDesc; i++)
        {
        descList.push_back(parseResult->GetParameterString("DescriptorList", i));
        }
    }
  costFunction->SetDescriptorList(descList);

  // Compute statistics of all the descriptors
  typedef VectorDataType::DataTreeType DataTreeType;
  typedef VectorDataType::DataNodeType DataNodeType;
  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType>
  TreeIteratorType;

  std::vector<double> accFirstOrderPS, accSecondOrderPS, minPS, maxPS;
  accFirstOrderPS.resize(descList.size());
  accSecondOrderPS.resize(descList.size());
  std::fill(accFirstOrderPS.begin(), accFirstOrderPS.end(), 0);
  std::fill(accSecondOrderPS.begin(), accSecondOrderPS.end(), 0);
  minPS.resize(descList.size());
  maxPS.resize(descList.size());
  unsigned int accNbElemPS = 0;

  TreeIteratorType itVectorPS(psReader->GetOutput()->GetDataTree());
  for( itVectorPS.GoToBegin(); !itVectorPS.IsAtEnd(); ++itVectorPS )
    {
    if (!itVectorPS.Get()->IsRoot()
        && !itVectorPS.Get()->IsDocument()
        && !itVectorPS.Get()->IsFolder())
      {
      DataNodeType::Pointer currentGeometry = itVectorPS.Get();

      for (unsigned int i = 0; i < descList.size(); ++i)
        {
        double desc = currentGeometry->GetFieldAsDouble(descList[i]);

        accFirstOrderPS[i]  += desc;
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
      accNbElemPS ++;
      }
    }

  TreeIteratorType itVectorNS(nsReader->GetOutput()->GetDataTree());
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

  for( itVectorNS.GoToBegin(); !itVectorNS.IsAtEnd(); ++itVectorNS )
    {
    if (!itVectorNS.Get()->IsRoot()
        && !itVectorNS.Get()->IsDocument()
        && !itVectorNS.Get()->IsFolder())
      {
      DataNodeType::Pointer currentGeometry = itVectorNS.Get();

      for (unsigned int i = 0; i < descList.size(); ++i)
        {
        double desc = currentGeometry->GetFieldAsDouble(descList[i]);

        accFirstOrderNS[i]  += desc;
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
      accNbElemNS ++;
      }
    }
  std::cout << "Descriptors Stats : " << std::endl;
  std::cout << "Positive Samples" << std::endl;
  for (unsigned int i = 0; i < descList.size(); ++i)
    {
    double mean = accFirstOrderPS[i] / accNbElemPS;
    double stddev = vcl_sqrt( accSecondOrderPS[i]/accNbElemPS - mean*mean );
    std::cout << descList[i] << "  :  " << mean << " +/- " << stddev
        << "  (min: " << minPS[i] << "  max: " << maxPS[i] << ")"<< std::endl;
    }

  std::cout << "Negative Samples" << std::endl;
  for (unsigned int i = 0; i < descList.size(); ++i)
    {
    double mean = accFirstOrderNS[i] / accNbElemNS;
    double stddev = vcl_sqrt( accSecondOrderNS[i]/accNbElemNS - mean*mean );
    std::cout << descList[i] << "  :  " << mean << " +/- " << stddev
        << "  (min: " << minNS[i] << "  max: " << maxNS[i] << ")"<< std::endl;
    }


  OptimizerType::ParametersType
  initialPosition( 4 * descList.size() );

  if (parseResult->IsOptionPresent("InitModel"))
    {
    for(unsigned int i=0; i<4; i++)
      {
      for(unsigned int j=0; j< descList.size(); j++)
        {
        initialPosition.SetElement(i+4*j,   otb::FuzzyDescriptorsModelManager::GetDescriptor(descList[j].c_str(), descMod).second[i]);
        }
      }
    }
  else
    {
    for(unsigned int j=0; j< descList.size(); j++)
      {
      initialPosition.SetElement((j*4),   std::min(minNS[j], maxPS[j]));
      initialPosition.SetElement((j*4)+2, std::max(minNS[j], maxPS[j]));
      initialPosition.SetElement((j*4)+1, 0.5*(initialPosition.GetElement((j*4)) + initialPosition.GetElement((j*4)+2)));
      initialPosition.SetElement((j*4)+3, 0.95);
      }
    }

  //Cost Function
  //Format Hypothesis
  LabelSetType Bhyp, Phyp;
  int nbSet;

  nbSet = parseResult->GetNumberOfParameters("BeliefSupport");

  for (int i = 0; i < nbSet; i++)
    {
    std::string str = parseResult->GetParameterString("BeliefSupport", i);
    Bhyp.insert(str);
    }
  costFunction->SetBeliefHypothesis(Bhyp);
  nbSet = parseResult->GetNumberOfParameters("PlausibilitySupport");
  for (int i = 0; i < nbSet; i++)
    {
    std::string str = parseResult->GetParameterString("PlausibilitySupport", i);
    Phyp.insert(str);
    }
  costFunction->SetPlausibilityHypothesis(Phyp);

  if (parseResult->IsOptionPresent("Weighting"))
    {
    costFunction->SetWeight(parseResult->GetParameterDouble("Weighting"));
    }
  if (parseResult->IsOptionPresent("Criterion"))
    {
    costFunction->SetCriterionFormula(parseResult->GetParameterString("Criterion"));
    }

  costFunction->SetGTVectorData(psReader->GetOutput());
  costFunction->SetNSVectorData(nsReader->GetOutput());
  //Optimizer
  optimizer->SetCostFunction(costFunction);
  if (parseResult->IsOptionPresent("MaximumNumberOfIterations"))
    {
    optimizer->SetMaximumNumberOfIterations(parseResult->GetParameterInt("MaximumNumberOfIterations"));
    }
  else
    {
    optimizer->SetMaximumNumberOfIterations(200);
    }
  /*
  optimizer->SetParametersConvergenceTolerance( 0.01 );
  optimizer->SetFunctionConvergenceTolerance(0.001);
  */
  OptimizerType::ParametersType
  simplexDelta( costFunction->GetNumberOfParameters() );
  simplexDelta.Fill(0.1);

  optimizer->AutomaticInitialSimplexOff();
  optimizer->SetInitialSimplexDelta( simplexDelta );

  optimizer->SetInitialPosition(initialPosition);

  // Create the Command observer and register it with the optimizer.
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  if (parseResult->IsOptionPresent("OptimizerObserver"))
    {
    optimizer->AddObserver( itk::IterationEvent(), observer );
    }

  try
  {
    // do the optimization
    optimizer->StartOptimization();
  }
  catch( itk::ExceptionObject& err )
  {
    // An error has occurred in the optimization.
    // Update the parameters
    std::cout << "ERROR: Exception Catched!" << std::endl;
    std::cout << err.GetDescription() << std::endl;
    const unsigned int numberOfIterations
    = optimizer->GetOptimizer()->get_num_evaluations();
    std::cout << "numberOfIterations : " << numberOfIterations << std::endl;
    std::cout << "Results : " << optimizer->GetCurrentPosition() << std::endl;
  }
  // get the results
  const unsigned int numberOfIterations
  = optimizer->GetOptimizer()->get_num_evaluations();
  std::cout << "numberOfIterations : " << numberOfIterations << std::endl;
  std::cout << "Results : " << optimizer->GetCurrentPosition() << std::endl;

  otb::FuzzyDescriptorsModelManager::DescriptorsModelType model;

  for (unsigned int i=0; i < descList.size(); i++)
    {
    otb::FuzzyDescriptorsModelManager::ParameterType        tmpParams;
    for (unsigned int j = 0; j<4; j++)
      {
      tmpParams.push_back(optimizer->GetCurrentPosition()[(i*4)+j]);
      }
    otb::FuzzyDescriptorsModelManager::AddDescriptor(descList[i],
                                                     tmpParams,
                                                     model);
    }
  otb::FuzzyDescriptorsModelManager::Save(parseResult->GetParameterString("Output"),
                                          model);

  return EXIT_SUCCESS;
}
