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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorDataToRandomLineGenerator.h"

#include "otbVectorDataProjectionFilter.h"  //Envelope TO WGS84 (OSM friendly)
#include "otbVectorDataIntoImageProjectionFilter.h" //WGS84 to index

#include "otbVectorDataToRoadDescriptionFilter.h"
#include "otbVectorDataToDSValidatedVectorDataFilter.h"

#include "itkAmoebaOptimizer.h"
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


//TESTING
#include "otbVectorDataFileWriter.h"


//namespace otb
//{

int otb::DSFuzzyModelEstimation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("DSFuzzyModelEstimation");
  descriptor->SetDescription("Estimate feature fuzzy model parameters using an image and an VectorData");
  descriptor->AddOption("InputImage", "Support to estimate the models on",
                        "in", 1, true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("InputVectorData", "Ground Truth Vector Data",
                        "vdin", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Hypothesis", "Dempster Shafer study hypothesis",
                        "hyp", 2, false, ApplicationDescriptor::StringList);
  //descriptor->AddOption("Criterion", "Dempster Shafer Criterion (by default (Belief+Plausibility)/2 >= 0.5))",
  //                      "cri", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("weighting", "Coefficient between 0 and 1 to promote false detections or undetection (default 0.5)",
                        "wgt", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("MaximumNumberOfIterations", "Maximum Number os Optimizer Iteration",
                        "MaxNbIt", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("DEMDirectory", "DEM directory",
                        "dem", 1, false, ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("OptimizerObserver", "Activate or not the optimizer observer",
                        "OptObs", 1, true, ApplicationDescriptor::Integer);

  //TESTING PURPOSE
  descriptor->AddOption("Output", "Output Model File Name",
                        "out", 1, true, ApplicationDescriptor::FileName);
  return EXIT_SUCCESS;
}


int otb::DSFuzzyModelEstimation::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorData<double>                     VectorDataType;
  typedef VectorDataType::ValuePrecisionType          PrecisionType;
  typedef VectorDataType::PrecisionType               CoordRepType;
  typedef otb::VectorDataFileReader<VectorDataType>   VectorDataReaderType;
  typedef otb::VectorDataToRandomLineGenerator<VectorDataType>
                                                      RandomGeneratorType;

  typedef otb::VectorImage<PrecisionType , 2>         ImageType;
  typedef otb::ImageFileReader<ImageType>             ImageReaderType;

  typedef otb::ImageToEnvelopeVectorDataFilter<ImageType, VectorDataType>
                                                      EnvelopeFilterType;

  typedef otb::VectorDataProjectionFilter<VectorDataType, ImageType>
                                                      VectorDataProjFilter;

  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, ImageType>
                                                      VectorDataReProjFilter;

  typedef otb::VectorDataToRoadDescriptionFilter<VectorDataType, ImageType>
                                                      DescriptionFilterType;

  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                                      ValidationFilterType;

  typedef otb::StandardDSCostFunction<ValidationFilterType>
                                                      CostFunctionType;
  typedef CostFunctionType::LabelSetType              LabelSetType;

  typedef itk::AmoebaOptimizer                        OptimizerType;

  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataWriterType;


  //Instantiate
  ImageReaderType::Pointer                   imgReader = ImageReaderType::New();
  VectorDataReaderType::Pointer               vdReader = VectorDataReaderType::New();
  EnvelopeFilterType::Pointer           envelopeFilter = EnvelopeFilterType::New();
  RandomGeneratorType::Pointer       vdRandomGenerator = RandomGeneratorType::New();
  VectorDataReProjFilter::Pointer     vdReProjFilterGT = VectorDataReProjFilter::New();
  VectorDataReProjFilter::Pointer     vdReProjFilterRL = VectorDataReProjFilter::New();
  DescriptionFilterType::Pointer   descriptionFilterGT = DescriptionFilterType::New();
  DescriptionFilterType::Pointer   descriptionFilterNS = DescriptionFilterType::New();
  CostFunctionType::Pointer               costFunction = CostFunctionType::New();
  OptimizerType::Pointer                     optimizer = OptimizerType::New();

  VectorDataWriterType::Pointer               vdWriter = VectorDataWriterType::New();

  //Read the image
  imgReader->SetFileName(parseResult->GetParameterString("InputImage"));
  imgReader->UpdateOutputInformation();

  imgReader->SetGlobalWarningDisplay(0);

  //Read the vector data
  vdReader->SetFileName(parseResult->GetParameterString("InputVectorData"));
  vdReader->Update();

  //Generate the envelope
  envelopeFilter->SetInput(imgReader->GetOutput()); //->Output in WGS84
  envelopeFilter->Update();

  //Generate Random lines
  vdRandomGenerator->SetInput(envelopeFilter->GetOutput());
  vdRandomGenerator->SetNumberOfOutputLine(vdReader->GetOutput()->Size());
  vdRandomGenerator->SetMinLineSize(4);
  vdRandomGenerator->SetMaxLineSize(20);

  //Reproject into image index coordinates
  vdReProjFilterGT->SetInputImage(imgReader->GetOutput());
  vdReProjFilterGT->SetInputVectorData(vdReader->GetOutput());
  vdReProjFilterGT->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
  vdReProjFilterGT->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilterGT->Update();

  vdReProjFilterRL->SetInputImage(imgReader->GetOutput());
  vdReProjFilterRL->SetInputVectorData(vdRandomGenerator->GetOutput());
  vdReProjFilterRL->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
  vdReProjFilterRL->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilterRL->Update();

  //Add Description to VectorDatas
  descriptionFilterGT->SetInput(vdReProjFilterGT->GetOutput());
  descriptionFilterGT->AddOpticalImage(imgReader->GetOutput());
  descriptionFilterGT->Update();
  descriptionFilterNS->SetInput(vdReProjFilterRL->GetOutput());
  descriptionFilterNS->AddOpticalImage(imgReader->GetOutput());
  descriptionFilterNS->Update();

  //Cost Function
  //Format Hypothesis
  LabelSetType hyp;
  if (parseResult->IsOptionPresent("Hypothesis"))
    {
    int NumberOfChannel = parseResult->GetNumberOfParameters("Hypothesis");
    for (int i = 0; i < NumberOfChannel; i++)
       {
        std::string str = parseResult->GetParameterString("Hypothesis", i);
        hyp.insert(str);
       }
    costFunction->SetHypothesis(hyp);
    }
  if (parseResult->IsOptionPresent("Weighting"))
    {
    costFunction->SetWeight(parseResult->GetParameterDouble("Weighting"));
    }
  costFunction->SetGTVectorData(descriptionFilterGT->GetOutput());
  costFunction->SetNSVectorData(descriptionFilterNS->GetOutput());

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
  simplexDelta.Fill(.25);
  /*
  simplexDelta.SetElement(0, 0.25);
  simplexDelta.SetElement(1, 0.25);
  simplexDelta.SetElement(2, 0.25);
  simplexDelta.SetElement(3, 0.20);
  simplexDelta.SetElement(4, 0.25);
  simplexDelta.SetElement(5, 0.25);
  simplexDelta.SetElement(6, 0.25);
  simplexDelta.SetElement(7, 0.20);
  */
  optimizer->AutomaticInitialSimplexOff();
  optimizer->SetInitialSimplexDelta( simplexDelta );

  //optimizer->SetScales()

  OptimizerType::ParametersType
        initialPosition( costFunction->GetNumberOfParameters() );
  //initialPosition.Fill(0.25);
  initialPosition.SetElement(0, 0.25);
  initialPosition.SetElement(1, 0.50);
  initialPosition.SetElement(2, 0.75);
  initialPosition.SetElement(3, 0.99);
  initialPosition.SetElement(4, 0.25);
  initialPosition.SetElement(5, 0.50);
  initialPosition.SetElement(6, 0.75);
  initialPosition.SetElement(7, 0.99);

  //initialPosition.SetElement();
  optimizer->SetInitialPosition(initialPosition);

  // Create the Command observer and register it with the optimizer.
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  if (parseResult->GetParameterInt("OptimizerObserver"))
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
  otb::FuzzyDescriptorsModelManager::ParameterType        ndvi, radiom;

  for (unsigned int i = 0; i<4; i++)
    {
    ndvi.push_back(optimizer->GetCurrentPosition()[i]);
    }
  for (unsigned int i = 0; i<4; i++)
    {
    radiom.push_back(optimizer->GetCurrentPosition()[i+4]);
    }
  otb::FuzzyDescriptorsModelManager::AddDescriptor("NDVI", ndvi, model);
  otb::FuzzyDescriptorsModelManager::AddDescriptor("RADIOM", radiom, model);
  otb::FuzzyDescriptorsModelManager::Save(parseResult->GetParameterString("Output"),
                                          model);

  return EXIT_SUCCESS;
}

//}

